/*
 * A118358  Records in A086793.
 * https://oeis.org/A118358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n+9) for n >= 6; a(1)=5, a(2)=9, a(3)=11, a(4)=12,
 *            a(5)=13
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)11;
    if (n == 4) return (seq_t)12;
    if (n == 5) return (seq_t)13;
    /* for n >= 6 */
    return (seq_t)n + (seq_t)9;
}
