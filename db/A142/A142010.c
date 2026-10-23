/*
 * A142010  Primes congruent to 6 mod 31.
 * https://oeis.org/A142010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 221, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime congruent to 6 (mod 31). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset starts at 1; defensive */

    /* Rough upper bound; will double if insufficient. */
    idx_t limit = n * 90 + 10;          /* about 90 * n, increased if needed */

    while (1) {
        /* Allocate sieve array (cast to avoid C++ warning). */
        unsigned char *sieve = (unsigned char *)calloc(limit + 1, sizeof(unsigned char));
        if (!sieve)
            return 0;                  /* allocation failure, unlikely */

        /* 0 and 1 are not prime */
        if (limit >= 0) sieve[0] = 0;
        if (limit >= 1) sieve[1] = 0;
        /* Mark all other entries as potential primes */
        for (idx_t i = 2; i <= limit; ++i)
            sieve[i] = 1;

        /* Sieve of Eratosthenes */
        for (idx_t p = 2; p * p <= limit; ++p) {
            if (sieve[p]) {
                for (idx_t m = p * p; m <= limit; m += p)
                    sieve[m] = 0;
            }
        }

        /* Count primes ≡ 6 (mod 31) */
        idx_t cnt = 0;
        for (idx_t cand = 6; cand <= limit; cand += 31) {
            if (sieve[cand]) {
                ++cnt;
                if (cnt == n) {
                    seq_t result = (seq_t)cand;
                    free(sieve);
                    return result;
                }
            }
        }

        /* Not enough primes found: enlarge limit and try again */
        free(sieve);
        limit *= 2;
    }
}
