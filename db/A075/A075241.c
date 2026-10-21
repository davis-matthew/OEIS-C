/*
 * A075241  Least base for which the n-th prime has no prime reversal in that
 *          base.
 * https://oeis.org/A075241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the least base b such that the n-th prime has no prime *
 *            reversal in base b
 * Range:     u8 n <= 43, u16 n <= 4506, u32 n <= 7257, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache for primes */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_limit = 0;

    /* grow the primes table if needed */
    if (n > primes_cnt) {
        /* compute a sufficient limit */
        idx_t limit = _prime_limit_for_idx(n);
        if (limit > primes_limit) {
            uint64_t *new_primes = NULL;
            size_t new_cnt = 0;
            build_primes_upto(limit, &new_primes, &new_cnt);
            free(primes);
            primes = new_primes;
            primes_cnt = new_cnt;
            primes_limit = limit;
        }
    }

    /* the n-th prime (1‑based index) */
    uint64_t P = primes[n - 1];

    /* try each base from 2 upward until a base with a composite reversal is found */
    for (idx_t base = 2; base <= P; ++base) {
        /* collect the digits of P in base 'base' */
        uint32_t digits[64];
        size_t len = 0;
        uint64_t tmp = P;
        if (tmp == 0) digits[len++] = 0;  /* keep for P==0, not used here */
        while (tmp > 0) {
            digits[len++] = (uint32_t)(tmp % (uint64_t)base);
            tmp /= (uint64_t)base;
        }

        /* compute the reversed value */
        seq_t rev = 0;
        for (size_t i = 0; i < len; ++i) {
            rev = rev * (seq_t)base + (seq_t)digits[i];
        }

        /* 1 or 0 is not prime, otherwise check primality */
        if (rev < 2) return (seq_t)base;
        if (!is_prime_big(rev)) return (seq_t)base;
    }

    /* The loop always succeeds because base == prime itself yields rev=1. */
    return (seq_t)P;
}
