/*
 * A238768  Number of n X 1 0..3 arrays with no element equal to the sum
 *          modulo 4 of elements to its left or elements above it.
 * https://oeis.org/A238768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 2*a(n-3) for n>=4; a(1)=3, a(2)=6,
 *            a(3)=14
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 53, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Seq_t constants are cast explicitly to avoid type mismatches. */
    if (n < 1) return (seq_t)0;        /* undefined for n=0, but safe to return 0 */

    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)14;

    /* Use iteratively the last three values to avoid complex memory handling. */
    seq_t a1 = (seq_t)3;   /* a(1) */
    seq_t a2 = (seq_t)6;   /* a(2) */
    seq_t a3 = (seq_t)14;  /* a(3) */
    seq_t a4;

    for (idx_t i = 4; i <= n; ++i) {
        a4 = a3 + (seq_t)2 * a2 + (seq_t)2 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}
