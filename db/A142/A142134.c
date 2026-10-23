/*
 * A142134  Primes congruent to 25 mod 37.
 * https://oeis.org/A142134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 25 (mod 37)
 * Range:     u8 n <= 1, u16 n <= 186, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the provided sieve_primes routine.
   It repeatedly increases the sieve limit until the nth prime
   in the desired residue class is found. */

#include <stdlib.h>
#include "../utils/sieve_primes.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* Offset starts at 1, but guard against 0 */

    /* Rough upper bound: n * 400 + 100
       (overkill for n <= 10,000). */
    idx_t limit = (n < 10) ? 200 : n * 400 + 100;

    idx_t *prime_arr = NULL;
    size_t prime_cnt = 0;

    while (1) {
        sieve_primes(limit, &prime_arr, &prime_cnt);

        idx_t k = 0;
        seq_t  result = 0;

        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_arr[i];
            if (p % 37 == 25) {
                ++k;
                if (k == n) {
                    result = (seq_t)p;
                    break;
                }
            }
        }

        free(prime_arr);

        if (k >= n) {
            return result;            /* Found the nth residue-class prime */
        }

        /* Not enough primes yet – enlarge the limit */
        limit *= 2;
    }
}
