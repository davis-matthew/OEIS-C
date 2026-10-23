/*
 * A016964  a(n) = (6*n + 4)^8.
 * https://oeis.org/A016964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 4)^8
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 41, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid overflow when n is large. */
    seq_t n_seq = (seq_t)n;
    seq_t x = n_seq * (seq_t)6 + (seq_t)4;  /* (6*n + 4) */
    seq_t res = (seq_t)1;
    /* Multiply eight times to obtain the eighth power. */
    for (int i = 0; i < 8; ++i)
        res *= x;
    return res;
}
