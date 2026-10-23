/*
 * A008720  Molien series for 3-dimensional group [2,5] = *225.
 * https://oeis.org/A008720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + n*(9+5*(-1)^n) + 23*(-1)^n + 26)/40)
 * Range:     u8 n <= 9, u16 n <= 363, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t before arithmetic to avoid
       overflow of idx_t (which is uint64_t). */
    seq_t sn = (seq_t)n;
    seq_t N;

    /* Use parity to avoid negative intermediates. */
    if (n & 1U)            /* n is odd */
    {
        /* N = n^2 + 4*n + 3 */
        N = sn * sn + sn * 4 + 3;
    }
    else                   /* n is even */
    {
        /* N = n^2 + 14*n + 49 */
        N = sn * sn + sn * 14 + 49;
    }

    /* All intermediate values are non‑negative, so standard integer
       division performs the desired floor. */
    return N / 40;
}
