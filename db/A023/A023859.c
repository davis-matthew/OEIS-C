/*
 * A023859  a(n) = 1*t(n) + 2*t(n-1) + ... + k*t(n+1-k), where
 *          k=floor((n+1)/2), and t = A023532.
 * https://oeis.org/A023859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} i * t(n + 1 – i) where t(m) = 1
 *            except when m = k*(k+3)/2 (k ≥ 1), in which case t(m) = 0
 * Range:     u8 n <= 46, u16 n <= 734, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* k = floor((n+1)/2) */
    idx_t k = (n + 1) >> 1;

    /* base sum of weights: S(k) = k*(k+1)/2 */
    seq_t sum = ((seq_t)k * (k + 1)) / 2;

    /* lowest index of t that appears in the sum */
    idx_t lo = n + 1 - k;   /* n+1-k */

    /* subtract contributions for all m = k*(k+3)/2 inside [lo, n]   */
    for (idx_t j = 1; ; ++j)
    {
        /* use 128‑bit intermediate to avoid overflow */
        unsigned __int128 m128 = ((unsigned __int128)j * (j + 3)) >> 1; /* m = j*(j+3)/2 */

        if (m128 > n)
            break;
        if (m128 >= lo)
        {
            /* the weight for this term is i = n+1-m */
            sum -= (seq_t)(n + 1 - (idx_t)m128);
        }
    }

    return sum;
}
