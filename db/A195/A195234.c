/*
 * A195234  Number of lower triangles of a 4 X 4 0..n array with each element
 *          differing from all of its diagonal, vertical, antidiagonal and
 *          horizontal neighbors by one or less.
 * https://oeis.org/A195234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2277*n - 1403 for n > 1; a(1) = 1024
 * Range:     u8 none, u16 n <= 29, u32 n <= 200, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset handling */
    if (n == 1) return (seq_t)1024;
    if (n == 0) return (seq_t)0;    /* undefined, return 0 for safety */

    /* compute using a wider type to avoid overflow */
    unsigned long long tmp = 2277ULL * (unsigned long long)n - 1403ULL;
    return (seq_t)tmp;
}
