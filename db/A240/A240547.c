/*
 * A240547  Number of non-congruent solutions of x^2 + y^2 + z^2 + t^2 == 0
 *          mod n.
 * https://oeis.org/A240547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π p^e||n f(p^e) where f(2^e) = 2^(2e+1) (e≥1), f(p^e) =
 *            p^(2e-1) * (p^(e+1) + p^e – 1) (p odd, e≥1), and a(1) = 1. This
 *            implements the multiplicative formula
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 1364, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Internal power function: computes base^exp in seq_t, exp up to idx_t. */
static seq_t pow_seq_u64(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;        /* a(1)=1 */

    seq_t ans = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint32_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* Compute f(p^e) */
            seq_t term;
            if (p == 2) {
                /* 2^(2e+1) */
                term = pow_seq_u64(2, 2 * e + 1);
            } else {
                seq_t p_pow_e      = pow_seq_u64((seq_t)p, e);
                seq_t p_pow_e_plus1 = p_pow_e * (seq_t)p;
                seq_t p_pow_2e_minus1 = pow_seq_u64((seq_t)p, 2 * e - 1);
                term = p_pow_2e_minus1 *
                         (p_pow_e_plus1 + p_pow_e - 1);
            }
            ans *= term;
        }
    }
    if (temp > 1) {               /* Remaining prime factor */
        idx_t p = temp;
        uint32_t e = 1;
        seq_t term;
        if (p == 2) {
            term = pow_seq_u64(2, 2 * e + 1);
        } else {
            seq_t p_pow_e      = pow_seq_u64((seq_t)p, e);
            seq_t p_pow_e_plus1 = p_pow_e * (seq_t)p;
            seq_t p_pow_2e_minus1 = pow_seq_u64((seq_t)p, 2 * e - 1);
            term = p_pow_2e_minus1 *
                     (p_pow_e_plus1 + p_pow_e - 1);
        }
        ans *= term;
    }
    return ans;
}
