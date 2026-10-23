/*
 * A168125  a(n) = n^2*(n^7+1)/2.
 * https://oeis.org/A168125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2*(n^7+1)/2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 149, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n into seq_t for full precision */
    seq_t n_seq = (seq_t)n;

    /* n^2 */
    seq_t n_sq = n_seq * n_seq;

    /* n^3 = n^2 * n */
    seq_t n_cu = n_sq * n_seq;

    /* n^4 = n^2 * n^2 */
    seq_t n_4 = n_sq * n_sq;

    /* n^7 = n^3 * n^4 */
    seq_t n_7 = n_cu * n_4;

    /* n^7 + 1 */
    seq_t n_7p1 = n_7 + (seq_t)1;

    /* divide a factor by 2 first to avoid overflow */
    seq_t result;
    if ((n_seq & (seq_t)1) == (seq_t)0)   /* n even → n^2 even */
    {
        result = (n_sq >> 1) * n_7p1;
    }
    else                               /* n odd  → n^7+1 even */
    {
        result = n_sq * (n_7p1 >> 1);
    }

    return result;
}
