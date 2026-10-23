/*
 * A198481  Square root of the largest square dividing ((2n-1)!)^(2n-3).
 * https://oeis.org/A198481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A198481: Square root of the largest square dividing ((2n-1)!)^(2n-3)
 *  a(n) = ∏_{p ≤ 2n-1} p^⌊(2n-3)·v_p( (2n-1)! ) / 2⌋
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* lazily sieve primes up to the maximum 2n-1 encountered */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t  max_limit   = 0;
    static bool   initialized = false;

    idx_t limit = 2*n - 1;          /* n ≥ 1, limit ≤ 19999 for n ≤ 10000 */

    /* reseed the sieve if we need a larger range */
    if (!initialized || limit > max_limit) {
        size_t max = (size_t)limit;
        bool *is_prime = (bool*)calloc(max + 1, sizeof(bool));
        if (!is_prime) exit(EXIT_FAILURE);
        for (size_t i = 2; i <= max; ++i) is_prime[i] = true;

        for (size_t p = 2; p * p <= max; ++p) {
            if (!is_prime[p]) continue;
            for (size_t q = p * p; q <= max; q += p) is_prime[q] = false;
        }

        /* count primes and copy them */
        size_t cnt = 0;
        for (size_t i = 2; i <= max; ++i) if (is_prime[i]) ++cnt;
        uint64_t *new_primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
        if (!new_primes) exit(EXIT_FAILURE);
        size_t idx = 0;
        for (size_t i = 2; i <= max; ++i) if (is_prime[i]) new_primes[idx++] = i;

        free(is_prime);
        free(primes);
        primes   = new_primes;
        prime_cnt = cnt;
        max_limit = limit;
        initialized = true;
    }

    seq_t res = (seq_t)1;           /* start with 1 */

    /* loop over all primes ≤ limit */
    for (size_t i = 0; i < prime_cnt && primes[i] <= limit; ++i) {
        uint64_t p = primes[i];
        /* Legendre's formula for v_p((2n-1)!) */
        idx_t vp = 0;
        idx_t t = limit;
        while (t) { t /= p; vp += t; }

        idx_t e = ((2*n - 3ULL) * vp) / 2;   /* floor((2n-3)*vp/2) */

        if (e == 0) continue;

        seq_t pow_p = seq_pow((seq_t)p, e);
        res = seq_mul(res, pow_p);
    }

    return res;
}
