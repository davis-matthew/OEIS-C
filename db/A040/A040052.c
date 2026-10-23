/*
 * A040052  Continued fraction for sqrt(60).
 * https://oeis.org/A040052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=7; for n>0, a(n) = [1,2,1,14][(n-1) mod 4]
 * Range:     u8 n <= 8929, u16 n <= 8929, u32 n <= 8929, u64 n <= 8929,
 *            u128 n <= 8929, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)7;
    /* mod 4 via bitwise and, works for all unsigned idx_t */
    static const seq_t cycle[4] = {(seq_t)1, (seq_t)2, (seq_t)1, (seq_t)14};
    return cycle[(idx_t)(n - 1) & 3];
}
