/*
 * A228462  Number of arrays of maxima of three adjacent elements of some
 *          length 7 0..n array.
 * https://oeis.org/A228462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2/15)*n^5 + (7/6)*n^4 + (25/6)*n^3 + (19/3)*n^2 +
 *            (21/5)*n + 1
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 124, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For integer arithmetic we use a common denominator 30, i.e.
   a(n) = (4*n^5 + 35*n^4 + 125*n^3 + 190*n^2 + 126*n + 30) / 30        */
seq_t a(idx_t n)
{
    /* The sequence is defined for n≥1.  For n==0 return 0 (unused). */
    if (n == 0) return 0;

    /* 128‑bit arithmetic to avoid overflow while evaluating the numerator. */
    __int128 nn  = (__int128)n;           /* n */
    __int128 n2  = nn * nn;              /* n² */
    __int128 n3  = n2 * nn;              /* n³ */
    __int128 n4  = n3 * nn;              /* n⁴ */
    __int128 n5  = n4 * nn;              /* n⁵ */

    /* Numerator of the rational expression with denominator 30. */
    __int128 num = 4*n5 + 35*n4 + 125*n3 + 190*n2 + 126*nn + 30;

    /* Integer division – the result is exact. */
    unsigned long long val = (unsigned long long)(num / 30);

    /* Return the value cast to the build’s seq_t type. */
    return (seq_t)val;
}
