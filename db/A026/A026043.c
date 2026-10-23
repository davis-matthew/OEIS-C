/*
 * A026043  a(n) = dot_product(1,2,...,n)*(5,6,...,n,1,2,3,4).
 * https://oeis.org/A026043
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 - 9*n + 49)/6 (n >= 5)
 * Range:     u8 n <= 9, u16 n <= 59, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 5 the offset is undefined; return 0 or any value. */
    if (n < 5)
        return (seq_t)0;

    seq_t N = (seq_t)n;
    seq_t part = (seq_t)2 * N * N - (seq_t)9 * N + (seq_t)49;
    return N * part / (seq_t)6;
}
