/*
 * A100445  Bisection of A000255.
 * https://oeis.org/A100445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(n)=a(n-1)*(4n^2+6n)+1 for n>0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = 1; /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t factor = i_seq * ((seq_t)4 * i_seq + (seq_t)6); /* 4n^2+6n */
        x = x * factor + 1;
    }
    return x;
}
