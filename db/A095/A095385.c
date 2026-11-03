/*
 * A095385  Numbers k with property that k is a peak value in 3x+1 trajectory
 *          such that both k+1 and k-1 are prime numbers.
 * https://oeis.org/A095385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A095385
   A(n) = smallest even integer k such that
      - k-1 and k+1 are prime (twin primes),
      - k is a peak in its Collatz (3x+1) trajectory
        (i.e., no value in the trajectory exceeds k).
   The function searches k successively, memoising the results. */

/* standard headers */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* primality test from the utilities library */
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* ---------- helper: peak test ---------- */
static bool is_peak(seq_t k)
{
    seq_t cur = k;
    /* In the trajectory starting from k, k must be the maximum. */
    while (cur != 1) {
        /* Collatz step */
        if ((cur % 2) == 0)
            cur = cur / 2;
        else
            cur = cur * 3 + 1;

        /* If we ever exceed k, then k is not a peak. */
        if (cur > k)
            return false;
    }
    return true;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *cache         = NULL;
    static idx_t cache_len      = 0;
    static idx_t cache_cap      = 0;
    static seq_t next_k         = (seq_t)4;  /* first candidate */
    static idx_t found          = 0;

    /* return cached term if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* enlarge cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < n)
            new_cap *= 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp)  /* safe fallback */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* search until we have found n terms */
    while (found < n) {
        /* twin‑prime test */
        if (isPrimeGeneric(next_k - 1) && isPrimeGeneric(next_k + 1)) {
            /* peak test */
            if (is_peak(next_k))
                cache[found++] = next_k;
        }
        /* advance to next even integer */
        next_k = next_k + 2;
    }
    cache_len = found;
    return cache[n - 1];
}
