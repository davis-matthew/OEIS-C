/*
 * A101638  Number of distinct 4-almost primes A014613 which are factors of n.
 * https://oeis.org/A101638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (#primes with exponent ≥4) + (#primes with exponent
 *            ≥3)·(j−1) + C(#primes with exponent ≥2, 2) + (#primes with
 *            exponent ≥2)·C(j−1, 2) + C(j, 4) where j is the number of
 *            distinct prime divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    uint64_t tmp = (uint64_t)n;
    unsigned j = 0;               /* number of distinct prime divisors */
    unsigned cnt_ge2 = 0;         /* #primes with exponent ≥2 */
    unsigned cnt_ge3 = 0;         /* #primes with exponent ≥3 */
    unsigned cnt_ge4 = 0;         /* #primes with exponent ≥4 */

    /* trial division factorisation */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            ++j;
            unsigned e = 0;
            while (tmp % p == 0) {
                ++e;
                tmp /= p;
            }
            if (e >= 2) ++cnt_ge2;
            if (e >= 3) ++cnt_ge3;
            if (e >= 4) ++cnt_ge4;
        }
    }
    if (tmp > 1) {                /* remaining prime factor */
        ++j;
        /* exponent of remaining prime is 1, so no increments needed */
    }

    seq_t res = 0;

    /* 1. prime powers 4 or more */
    if (cnt_ge4) res += (seq_t)cnt_ge4;

    /* 2. 3+1 combinations */
    if (j >= 2 && cnt_ge3) res += (seq_t)cnt_ge3 * (seq_t)(j - 1);

    /* 3. 2+2 combinations */
    if (cnt_ge2 >= 2) res += (seq_t)cnt_ge2 * (seq_t)(cnt_ge2 - 1) / 2;

    /* 4. 2+1+1 combinations */
    if (cnt_ge2 >= 1 && j >= 3) {
        seq_t comb = (seq_t)(j - 1) * (seq_t)(j - 2) / 2;   /* C(j-1,2) */
        res += (seq_t)cnt_ge2 * comb;
    }

    /* 5. 1+1+1+1 combinations */
    if (j >= 4)
        res += (seq_t)j * (seq_t)(j - 1) * (seq_t)(j - 2) * (seq_t)(j - 3) / 24;

    return res;
}
