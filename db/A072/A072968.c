/*
 * A072968  Least k>0 such that the last digit of (n+k)^(n+k) is the same as
 *          the last digit of n^n.
 * https://oeis.org/A072968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest k>0 so that the last digit of (n+k)^(n+k) equals
 *            the last digit of n^n
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   last_digit_from_remainders
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/last_digit_from_remainders.h"

seq_t a(idx_t n)
{
    /* Pre‑compute last digit of n^n using remainders. */
    uint8_t d_n = (uint8_t)(n % 10);
    idx_t  n_mod4 = n % 4;
    idx_t  n_mod2 = n % 2;
    seq_t target = last_digit_from_remainders(d_n, n_mod4, n_mod2);

    /* Explore k=1…20 – the sequence is periodic with period 20. */
    for (idx_t k = 1; k <= 20; ++k) {
        uint8_t d_m = (uint8_t)((d_n + (k % 10)) % 10);
        idx_t  m_mod4 = (n_mod4 + (k % 4)) % 4;
        idx_t  m_mod2 = (n_mod2 + (k % 2)) % 2;
        seq_t cur = last_digit_from_remainders(d_m, m_mod4, m_mod2);
        if (cur == target)
            return (seq_t)k;
    }

    /* For safety – this should never happen. */
    return (seq_t)0;
}
