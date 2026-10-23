/*
 * A040770  Continued fraction for sqrt(799).
 * https://oeis.org/A040770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=28; a(n)=3,1,3,56 periodic for n>0: a(n)=cycle[(n-1)%4]
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)28;
    static const seq_t cycle[4] = { (seq_t)3, (seq_t)1, (seq_t)3, (seq_t)56 };
    return cycle[(n - 1) & 3];
}
