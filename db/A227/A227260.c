/*
 * A227260  Number of nX3 0,1 arrays indicating 2X2 subblocks of some larger
 *          (n+1)X4 binary array having a sum of two or less, with rows and
 *          columns of the latter in lexicographically nondecreasing order.
 * https://oeis.org/A227260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/90720)*n^9 + (11/40320)*n^8 + (53/30240)*n^7 + *
 *            (73/2880)*n^6 + (301/4320)*n^5 - (2683/5760)*n^4 * +
 *            (448811/90720)*n^3 + (1613/1120)*n^2 - (83207/2520)*n + 33 *
 *            (valid for n > 5). Small n are supplied explicitly
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly. */
    if (n <= 5) {
        static const seq_t tbl[5] = {4, 23, 98, 353, 1111};
        return tbl[n - 1];
    }

    /* Use 128‑bit intermediates to avoid overflow for 64‑bit builds.
       For arbitrary‑precision seq_t the intermediate type works as a
       temporary and is cast back to seq_t on return. */
#if defined(__SIZEOF_INT128__) || defined(__INT128__) || defined(__int128)
    unsigned __int128 nn = n;
    unsigned __int128 n2 = nn * nn;     /* n² */
    unsigned __int128 n3 = n2 * nn;     /* n³ */
    unsigned __int128 n4 = n3 * nn;     /* n⁴ */
    unsigned __int128 n5 = n4 * nn;     /* n⁵ */
    unsigned __int128 n6 = n5 * nn;     /* n⁶ */
    unsigned __int128 n7 = n6 * nn;     /* n⁷ */
    unsigned __int128 n8 = n7 * nn;     /* n⁸ */
    unsigned __int128 n9 = n8 * nn;     /* n⁹ */

    /* Numerator of the polynomial, expressed with integer coefficients
       obtained from the original rational expression. */
    unsigned __int128 numerator =
        4ull   * n9
      + 99ull  * n8
      + 636ull * n7
      + 9198ull * n6
      + 25284ull * n5
      - 169029ull * n4
      + 1795244ull * n3
      + 522612ull * n2
      - 11981808ull * nn
      + 11975040ull;

    /* Divide by 362880 (= 90720×4) to obtain the exact term. */
    return (seq_t)(numerator / 362880ull);
#else
    /* Fallback for environments without 128‑bit integer support.
       The same arithmetic is performed in the built‑in integer type
       used by seq_t; the compiler will promote everything to
       unsigned __int128 if the underlying type is 64‑bit or larger. */
    seq_t n1 = (seq_t)n;
    seq_t n2 = n1 * n1;
    seq_t n3 = n2 * n1;
    seq_t n4 = n3 * n1;
    seq_t n5 = n4 * n1;
    seq_t n6 = n5 * n1;
    seq_t n7 = n6 * n1;
    seq_t n8 = n7 * n1;
    seq_t n9 = n8 * n1;

    seq_t pos = (4ull * n9 + 99ull * n8 + 636ull * n7
                + 9198ull * n6 + 25284ull * n5
                + 1795244ull * n3 + 522612ull * n2
                + 11975040ull);
    seq_t neg = 169029ull * n4 + 11981808ull * n1;

    seq_t numerator = pos - neg;
    return numerator / 362880ull;
#endif
}
