/*
 * A060983  Number of primitive sublattices of index n in generic
 *          3-dimensional lattice.
 * https://oeis.org/A060983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d^3 | n} mu(d) * d(n/d^3) computed via the
 *            multiplicative formula for prime powers. The sequence is
 *            multiplicative and for a prime power p^e the contribution is
 *            a(p^e) = S(e) - S(e-3) where S(e) = Σ_{k=0..e} p^{2k} *
 *            (p^{e-k+1}-1)/(p-1)
 * Range:     u8 n <= 11, u16 n <= 143, u32 n <= 3220, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* multiplicative contribution of a prime power p^e */
static seq_t prime_power_contrib(idx_t p, idx_t e)
{
    /* S(e) */
    seq_t sum_e = 0;
    seq_t p_seq = (seq_t)p;
    seq_t p_sq  = p_seq * p_seq;
    seq_t p_minus1 = p_seq - 1;

    seq_t tmp  = big_pow(p_seq, e + 1);   /* p^(e+1) */
    seq_t pow2k = (seq_t)1;

    for (idx_t k = 0; k <= e; ++k) {
        seq_t inner = tmp - 1;
        seq_t term  = pow2k * inner / p_minus1;
        sum_e += term;

        pow2k *= p_sq;
        tmp   /= p_seq;   /* exact division by p */
    }

    /* S(e-3) , zero if e<3 */
    seq_t sum_e3 = 0;
    if (e >= 3) {
        idx_t e3 = e - 3;
        seq_t tmp3  = big_pow(p_seq, e3 + 1);
        seq_t pow2k3 = (seq_t)1;

        for (idx_t k = 0; k <= e3; ++k) {
            seq_t inner = tmp3 - 1;
            seq_t term  = pow2k3 * inner / p_minus1;
            sum_e3 += term;

            pow2k3 *= p_sq;
            tmp3   /= p_seq;
        }
    }

    return sum_e - sum_e3;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but guard */

    idx_t temp = n;
    seq_t result = (seq_t)1;

    /* factor 2 */
    idx_t e = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++e;
    }
    if (e > 0) {
        result *= prime_power_contrib(2, e);
    }

    /* odd factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            result *= prime_power_contrib(p, e);
        }
    }

    /* remaining prime factor >1 */
    if (temp > 1) {
        result *= prime_power_contrib(temp, 1);
    }

    return result;
}
