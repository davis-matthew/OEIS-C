/*
 * A014904  a(1)=1, a(n) = 20*a(n-1) + n.
 * https://oeis.org/A014904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) + n, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  The sequence grows monotonically, so a(1) is the first term
        and a(n) is defined for n ≥ 1.  */
    if (n < 1) {
        /* Undefined for n < 1; return 0 as a sentinel. */
        return (seq_t)0;
    }

    seq_t result = (seq_t)1;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        result = result * (seq_t)20 + (seq_t)i;
    }
    return result;
}
