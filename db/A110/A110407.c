/*
 * A110407  Integers with mutual residues -2.
 * https://oeis.org/A110407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 + 2*a(n-1) - 2 for n>3 Initial values: a(1)=3,
 *            a(2)=5, a(3)=13
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly.  */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;

    seq_t val = (seq_t)13;           /* a(3) */
    if (n == 3) return val;

    /* Compute iteratively: a(k)=a(k-1)^2+2*a(k-1)-2 for k=4..n */
    for (idx_t k = 4; k <= n; ++k) {
        val = val * val + (seq_t)2 * val - (seq_t)2;
    }

    return val;
}
