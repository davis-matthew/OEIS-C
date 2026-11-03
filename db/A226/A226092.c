/*
 * A226092  Fourth powers that become prime when their most significant
 *          (leftmost) decimal digit is removed.
 * https://oeis.org/A226092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-03)
 * Helpers:   ipow_idx, is_prime_u128, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A226092: the n-th fourth power k⁴ whose most
   significant decimal digit is removed leaving a prime number.
   The function searches successive integers k, checks the condition,
   and memoises the found values so that any call order is fast. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper function prototypes supplied by the build environment */
#include <stdint.h>
#include <limits.h>
#include "../utils/ipow_idx.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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

/* Cached sequence values ------------------------------------------- */

/* Pointer to the cached array */
static seq_t *seq_cache = NULL;
/* Number of terms currently stored */
static size_t cache_len = 0;
/* Current allocated capacity of seq_cache */
static size_t cache_cap = 0;
/* The next integer k that still needs to be examined */
static idx_t next_k = 1;

/* Ensure that seq_cache can hold at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence (n >= 1) */
seq_t a(idx_t n)
{
    /* Undefined for offset 0, but the caller should never request it */
    if (n == 0) return 0;

    /* Grow the cache until it contains at least n terms */
    while (cache_len < (size_t)n)
    {
        /* Compute k⁴ */
        seq_t p = ipow_idx((seq_t)next_k, 4);

        /* Number of decimal digits of p */
        int d = num_digits(p);
        if (d > 0)
        {
            /* Remove the most significant digit: keep the last d-1 digits */
            seq_t mod = pow10((idx_t)(d - 1));
            seq_t r = p % mod;

            if (isPrimeGeneric(r))
            {
                /* Store p, expanding the cache if necessary */
                ensure_cache_capacity(cache_len + 1);
                seq_cache[cache_len++] = p;
            }
        }

        ++next_k;
    }

    return seq_cache[(size_t)n - 1];
}
