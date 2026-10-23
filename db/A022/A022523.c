/*
 * A022523  Nexus numbers (n + 1)^7 - n^7.
 * https://oeis.org/A022523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^7 - n^7
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 28, u64 n <= 1174,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n + (seq_t)1;  /* (n+1) as seq_t */
    seq_t pow1 = (seq_t)1;
    seq_t pow0 = (seq_t)1;

    /* compute (n+1)^7 and n^7 */
    for (int i = 0; i < 7; ++i)
    {
        pow1 *= n1;
        pow0 *= (seq_t)n;
    }

    return pow1 - pow0;
}
