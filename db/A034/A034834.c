/*
 * A034834  One seventh of sept-factorial numbers.
 * https://oeis.org/A034834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7^(n-1) * n! (for n >= 1, offset 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; // undefined for offset 1, but safe guard
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        res *= (seq_t)i;        /* multiply by i */
        if (i < n) res *= (seq_t)7; /* multiply by 7 for each of the first n-1 indices */
    }
    return res;
}
