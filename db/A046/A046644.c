/*
 * A046644  From square root of Riemann zeta function: form Dirichlet series
 *          Sum b_n/n^s whose square is zeta function; sequence gives
 *          denominator of b_n.
 * https://oeis.org/A046644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, nice, mult
 * Formula:   a(n) = \prod_{p^e\|n} 2^{v_2(e!)+e} (a(1)=1)
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Compute the exponent of 2 in e! (Legendre's formula) */
static idx_t v2_factorial(idx_t e)
{
    idx_t v = 0;
    while (e > 1) {
        e /= 2;
        v += e;
    }
    return v;
}

/* OEIS A046644 */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t temp = n;
    seq_t sum = 0;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            sum += (seq_t)(v2_factorial(e) + e);
        }
    }

    /* Remaining prime factor (if any) has exponent 1 */
    if (temp > 1)
        sum += (seq_t)(v2_factorial(1) + 1);

    return pow2((idx_t)sum);
}
