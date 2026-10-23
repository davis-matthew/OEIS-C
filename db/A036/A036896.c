/*
 * A036896  Odd refactorable numbers.
 * https://oeis.org/A036896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 29, u32 n <= 3504, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th odd refactorable number.
   An odd refactorable number is an odd integer n such that
   n is divisible by tau(n), the number of its positive divisors.
   All such numbers are squares.  This routine searches increasing
   odd squares m² (m odd) and keeps those that satisfy
   m² % tau(m²) == 0.  Cached results are reused across calls. */

#include <stdlib.h>
#include <stdint.h>

/* External linkage required by the build */
seq_t a(idx_t n)
{
    /* Static cache of sequence values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0, cache_cap = 0;
    /* Next odd m to test */
    static uint64_t next_m = 1;

    /* Initialise cache on first call */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_size = 0;
    }

    /* Generate terms until we have at least n of them */
    while (cache_size < n) {
        uint64_t m   = next_m;          /* current odd m */
        seq_t   square = (seq_t)m * (seq_t)m;    /* m² */

        /* Compute tau(m²) by factoring m */
        uint64_t tmp   = m;
        seq_t   tau    = 1;              /* tau will stay small */
        for (uint64_t p = 3; p * p <= tmp; p += 2) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            if (exp) {
                tau = tau * (seq_t)(2 * exp + 1);
            }
        }
        /* Remaining prime factor > 1 (must be odd) */
        if (tmp > 1) {
            tau = tau * 3;               /* exponent 1 -> 2*1+1 = 3 */
        }

        /* Check refactorable condition */
        if ((square % tau) == 0) {
            /* Ensure cache capacity */
            if (cache_size >= cache_cap) {
                cache_cap = cache_cap * 2 + 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = square;
        }
        /* Next odd m */
        next_m += 2;
    }

    return cache[n - 1];
}
