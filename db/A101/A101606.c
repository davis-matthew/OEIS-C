/*
 * A101606  a(n) = number of divisors of n that have exactly three (not
 *          necessarily distinct) prime factors.
 * https://oeis.org/A101606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = |{k : e_k ≥ 3}| + (j – 1)·|{k : e_k ≥ 2}| + C(j,3) where
 *            n = ∏ p_k^{e_k} and j is the number of distinct prime factors
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t tmp = n;
    idx_t j = 0;          /* number of distinct primes */
    idx_t cnt_ge2 = 0;    /* number of primes with exponent ≥ 2 */
    idx_t cnt_ge3 = 0;    /* number of primes with exponent ≥ 3 */

    /* trial division – safe for any 64‑bit n */
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            ++j;
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            if (e >= 2) ++cnt_ge2;
            if (e >= 3) ++cnt_ge3;
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1) {
        ++j;              /* exponent is 1, so not counted in cnt_ge2/3 */
    }

    seq_t result = 0;

    /* type (3) : p^3 */
    result += (seq_t)cnt_ge3;

    /* type (2,1) : choose one prime with e ≥ 2, one other prime */
    if (j > 1 && cnt_ge2 > 0) {
        result += (seq_t)(j - 1) * (seq_t)cnt_ge2;
    }

    /* type (1,1,1) : choose any 3 distinct primes */
    if (j >= 3) {
        seq_t t3 = (seq_t)j * (seq_t)(j - 1) * (seq_t)(j - 2);
        t3 /= 6;               /* C(j,3) = j*(j‑1)*(j‑2)/6 */
        result += t3;
    }

    return result;
}
