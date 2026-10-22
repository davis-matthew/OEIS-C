/*
 * A001044  a(n) = (n!)^2.
 * https://oeis.org/A001044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = (n!)^2; Recurrence: a(0) = 1, a(n) = n^2 * a(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        seq_t i_seq = (seq_t)i;
        result = i_seq * i_seq * result;
    }
    return result;
}
