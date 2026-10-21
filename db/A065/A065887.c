/*
 * A065887  Smallest number whose square is divisible by n!.
 * https://oeis.org/A065887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p ≤ n} p^{⌈vp(n!)/2⌉} (smallest integer whose square
 *            is divisible by n!)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 17, u64 n <= 30, u128 n <= 50,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"   /* provides build_primes_upto() */

seq_t a(idx_t n)
{
    /* Prime list cache: up to the largest n seen so far. */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_limit = 0;

    /* Rebuild prime cache if we need to handle a larger n. */
    if (n > prime_limit) {
        if (primes != NULL)
            free(primes);
        build_primes_upto(n, &primes, &prime_cnt);
        prime_limit = n;
    }

    seq_t result = 1;

    for (size_t i = 0; i < prime_cnt && primes[i] <= n; ++i) {
        uint64_t p = primes[i];

        /* Compute vp(n!) for prime p. */
        idx_t tmp = n;
        uint64_t exp = 0;
        while (tmp) {
            tmp /= p;
            exp += tmp;
        }

        /* How many copies of p are needed in a(n). */
        uint64_t need = (exp + 1) / 2;       /* ceil(exp/2) */

        /* Multiply result by p^need. */
        for (uint64_t j = 0; j < need; ++j)
            result *= (seq_t)p;
    }

    return result;
}
