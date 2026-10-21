/*
 * A216816  Duplicate of A122870.
 * https://oeis.org/A216816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the n-th prime p with p % 20 == 3 or 7
 * Range:     i8 n <= 10, i16 n <= 62, i32 n <= 62, i64 n <= 62, i128 n <= 62,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;           /* undefined for n < 1 */
    }

    /* Initial bound: 20 covers one full residue cycle */
    idx_t limit = 20 * n;
    if (limit < 100) {
        limit = 100;
    }

    while (1) {
        uint64_t *primes = NULL;
        size_t prime_cnt = 0;

        /* Retrieve all primes ≤ limit */
        build_primes_upto(limit, &primes, &prime_cnt);

        idx_t matched = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p < 3) {
                continue;
            }
            uint64_t r = p % 20;
            if (r == 3 || r == 7) {
                ++matched;
                if ((idx_t)matched == n) {
                    seq_t result = (seq_t)p;
                    free(primes);
                    return result;
                }
            }
        }

        free(primes);

        /* Increase search bound if necessary */
        if (limit > ((idx_t)-1) / 2) {
            /* Overflow protection – should not happen for n ≤ 10 000 */
            break;
        }
        limit *= 2;
    }

    /* Should not reach here for valid n */
    return 0;
}
