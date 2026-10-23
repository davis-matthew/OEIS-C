/*
 * A011929  a(n) = floor(n(n-1)(n-2)(n-3)/19).
 * https://oeis.org/A011929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)*(n-3)/19)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 4 the product is zero, matching the OEIS terms a(0) through a(3). */
    if (n < 4)
        return 0;

    seq_t ns = (seq_t)n;
    seq_t prod = ns * (ns - 1) * (ns - 2) * (ns - 3);
    return prod / (seq_t)19;
}
