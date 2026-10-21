/*
 * A065222  Fill a triangular array by rows by writing numbers 1 up to b(0), 1
 *          up to b(1), etc., where b(n) are the hexagonal numbers. The final
 *          elements of the rows form a(n).
 * https://oeis.org/A065222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the offset of the n‑th row's last element within the
 *            hexagonal segment that contains it. The position of the last
 *            element of row n in the concatenated sequence is P(n) =
 *            (n+1)(n+2)/2. Let T_m = Σ_{k=1}^m k(2k-1) be the cumulative
 *            length of the first m hexagonal segments. The unique m such...
 * Range:     u8 n <= 20, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t P = (n_seq + 1) * (n_seq + 2) / 2;   /* position of last element of row n */

    seq_t cum = 0;   /* cumulative length of processed hexagonal segments */

    for (idx_t m = 1; ; ++m)
    {
        seq_t m_seq = (seq_t)m;
        seq_t H = m_seq * ((seq_t)2 * m_seq - 1);   /* H(m) = m(2m-1) */
        cum += H;
        if (cum >= P)
        {
            /* cum - H is Σ_{k=1}^{m-1} H(k) */
            return P - (cum - H);
        }
    }
}
