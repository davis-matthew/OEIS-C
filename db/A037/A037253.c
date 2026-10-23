/*
 * A037253  a(n) = n^12*(n^8+n^4+1)*(n^6-1)*(n^2-1).
 * https://oeis.org/A037253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^12*(n^8+n^4+1)*(n^6-1)*(n^2-1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    seq_t n4 = n2 * n2;
    seq_t n6 = n4 * n2;
    seq_t n8 = n4 * n4;
    seq_t n12 = n8 * n4;
    seq_t t1 = n8 + n4 + (seq_t)1;
    seq_t t2 = n6 - (seq_t)1;
    seq_t t3 = n2 - (seq_t)1;
    return n12 * t1 * t2 * t3;
}
