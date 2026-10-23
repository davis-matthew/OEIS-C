/*
 * A102693  a(n) is the number of digraphs (not allowing loops) with vertices
 *          1,2,...,n that have a unique Eulerian tour (up to cyclic shift).
 * https://oeis.org/A102693
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2)(n+3)...(2n-1) (empty product means 1 for n=2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n < 2 the function is not defined by the OEIS offset,
       but returning 0 keeps the function safe for all idx_t values.   */
    if (n < 2)
        return 0;

    seq_t result = 1;
    idx_t start = n + 2;
    idx_t end   = n + n - 1;          /* 2*n - 1, computed without overflow for small n */

    for (idx_t k = start; k <= end; ++k)
        result *= (seq_t)k;

    return result;
}
