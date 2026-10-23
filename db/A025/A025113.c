/*
 * A025113  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (odd natural numbers), t = A000201 (lower Wythoff
 *          sequence).
 * https://oeis.org/A025113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} (2i-1) · ⌊(n-i+2)φ⌋, where φ =
 *            (1+√5)/2 and ⌊x⌋ is the integer part of x
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The lower Wythoff sequence t(m) = ⌊mφ⌋ can be computed as
   t(m) = ⌊(m + ⌊m√5⌋)/2⌋.  The helper floor_sqrt provides ⌊√x⌋. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t k = (n + 1) >> 1;            /* k = floor((n+1)/2) */

    for (idx_t i = 1; i <= k; ++i) {
        idx_t m_idx = n - i + 2;       /* m = n - i + 2   (>=1) */
        seq_t m = (seq_t)m_idx;        /* promote to seq_t */

        /* compute t(m) = floor((m + floor(m√5))/2) */
        seq_t sq5_mul = (seq_t)5 * m * m;       /* 5*m^2 */
        seq_t sqrt5 = floor_sqrt(sq5_mul);     /* floor(m√5) */
        seq_t t_val = (m + sqrt5) >> 1;        /* floor((m+floor(m√5))/2) */

        seq_t s_val = (seq_t)2 * i - 1;        /* 2i-1 */

        sum += s_val * t_val;
    }

    return sum;
}
