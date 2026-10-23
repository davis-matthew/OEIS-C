/*
 * A038793  T(n,n-3), array T as in A038792.
 * https://oeis.org/A038793
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 - 9*n^2 + 38*n - 42) / 6 for n >= 5, with a(3) = 1,
 *            a(4) = 4
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset and the first two special values */
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)4;
    if (n <= 2) return (seq_t)0;      /* undefined for n < 3, return 0 */

    /* Compute the formula using only unsigned arithmetic */
    seq_t nn  = (seq_t)n;            /* n */
    seq_t n2  = nn * nn;            /* n^2 */
    seq_t n3  = n2 * nn;            /* n^3 */

    /* tmp = n3 + 38*n - 42 - 9*n^2 (the numerator) */
    seq_t tmp = n3 + (seq_t)38 * nn;
    tmp -= (seq_t)42;
    tmp -= (seq_t)9 * n2;

    return tmp / 6;
}
