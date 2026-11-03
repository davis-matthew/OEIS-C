/*
 * A066074  Primes arising in A066073.
 * https://oeis.org/A066074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime appearing in the sequence of all σ(x)-1
 *            with x composite (x>1). The terms are listed in order of
 *            increasing x, and duplicates are kept. Order of appearance: for
 *            each integer x starting at 2, if x is composite, let p = σ(x)-1.
 *            If p is prime, record p. The first such prime is...
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 998 (2026-11-03)
 * Helpers:   is_prime, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
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

seq_t a(idx_t n)
{
    /* Memoization of the sequence values. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements already stored */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static idx_t next_x = 2;        /* next integer to test */

    if (n == 0) return 0;          /* undefined for offset 0, return 0 */

    /* Grow the cache until it contains at least n terms. */
    while (cache_size < (size_t)n) {
        /* Find the next composite x with σ(x)-1 prime. */
        for (;;) {
            /* Skip primes.  is_prime works on idx_t. */
            if (!is_prime(next_x)) {
                seq_t sigma = sigma_of(next_x);
                seq_t p = sigma - (seq_t)1;
                if (isPrimeGeneric(p)) {
                    /* Append p to the cache. */
                    if (cache_size == cache_cap) {
                        cache_cap = cache_cap ? cache_cap * 2 : 16;
                        cache = mem_resize(seq_t, cache, cache_cap);
                    }
                    cache[cache_size++] = p;
                    next_x++;  /* advance to next x for future calls */
                    break;
                }
            }
            next_x++;  /* try the next integer */
        }
    }

    return cache[n - 1];
}
