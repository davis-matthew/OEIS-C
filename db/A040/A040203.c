/*
 * A040203  Continued fraction for sqrt(218).
 * https://oeis.org/A040203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=14; for n≥1 the terms repeat 1,3,3,1,28
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)14;
    static const seq_t cycle[5] = {(seq_t)1, (seq_t)3, (seq_t)3, (seq_t)1, (seq_t)28};
    return cycle[(n - 1) % 5];
}
