/*
 * A027698  Numbers k such that the k-th prime has an odd number of 1's in its
 *          binary expansion.
 * https://oeis.org/A027698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A027698: a(n) = the index k of the n-th prime whose binary
   expansion contains an odd number of 1's.  The function performs an
   incremental sieve; no external utilities are required. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Popcount for 64‑bit integers.  Count of 1's in binary representation. */
static inline unsigned popcount_u64(uint64_t x)
{
    unsigned c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* outside the defined range */

    /* Start with a rough upper bound.  The n‑th prime with odd popcount
       is at most about 2 · n·log(2 n), so 20 × n is more than enough for
       n≤10 000.  Ensure a minimum bound for very small n. */
    idx_t limit = n * 20;
    if (limit < 100) limit = 100;

    while (1) {
        /* Allocate memory for the sieve.  char is used for minimal
           footprint; value 0 = prime, 1 = composite. */
        char *is_comp = (char *)malloc((size_t)(limit + 1));
        if (!is_comp) return 0;         /* allocation failure fallback */

        memset(is_comp, 0, (size_t)(limit + 1));

        /* Standard Eratosthenes sieve up to 'limit'. */
        for (idx_t p = 2; p * p <= limit; ++p)
            if (!is_comp[p])
                for (idx_t m = p * p; m <= limit; m += p)
                    is_comp[m] = 1;

        idx_t prime_idx = 0;   /* 1‑based index in the prime sequence */
        idx_t found     = 0;   /* count of primes with odd popcount */

        for (idx_t i = 2; i <= limit; ++i) {
            if (!is_comp[i]) {
                ++prime_idx;
                if (popcount_u64(i) & 1) {
                    ++found;
                    if (found == n) {
                        free(is_comp);
                        return (seq_t)prime_idx;
                    }
                }
            }
        }

        /* Not enough primes found – increase limit and repeat. */
        free(is_comp);
        limit *= 2;
    }
}
