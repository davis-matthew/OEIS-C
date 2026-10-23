/*
 * A121658  Primes neither of the n^2+1 nor n^2+n+1 form.
 * https://oeis.org/A121658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nth prime that is NOT of the form n^2+1 or n^2+n+1 (a(n)
 *            = 0 for n <= 0)
 * Range:     u8 n <= 1, u16 n <= 442, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

static bool is_forbidden(seq_t p) {
    /* p is prime, check if it equals n^2+1 or n^2+n+1 for some n */
    /* 1) n^2 + 1 -------------------------------------------------- */
    seq_t x = p - (seq_t)1;
    seq_t sx = floor_sqrt(x);
    if (sx * sx == x) return true;   /* p = n^2 + 1 */

    /* 2) n^2 + n + 1 ------------------------------------------------ */
    seq_t y = (seq_t)4 * p - (seq_t)3;   /* 4p-3 must be a square */
    seq_t sy = floor_sqrt(y);
    if (sy * sy == y) {                 /* square exists */
        /* check parity: (sqrt(4p-3)-1) must be even */
        if (((sy - (seq_t)1) & (seq_t)1) == 0) {
            return true;                /* p = n^2 + n + 1 */
        }
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    idx_t limit = n * 1000ULL + 100ULL;
    if (limit < 1000ULL) limit = 1000ULL;

    while (1) {
        uint64_t *primes = NULL;
        size_t prime_cnt = 0;
        build_primes_upto(limit, &primes, &prime_cnt);

        idx_t count = 0;
        seq_t result = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            seq_t p = (seq_t)primes[i];
            if (is_forbidden(p)) continue;
            ++count;
            if (count == n) {
                result = p;
                break;
            }
        }
        free(primes);

        if (count >= n) return result;

        /* Not enough terms found, increase limit and repeat */
        if (limit > ~(idx_t)0 / 2) break; /* avoid overflow */
        limit *= 2;
    }

    /* If we exit the loop (unlikely), return 0 */
    return 0;
}
