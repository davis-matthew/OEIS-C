/*
 * A239576  Number of non-equivalent (mod D_4) binary n X n matrices with 4
 *          pairwise nonadjacent 1's.
 * https://oeis.org/A239576
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 -30*n^6 +24*n^5 +352*n^4 -576*n^3 -1280*n^2 +3360*n
 *            -1536 + IF(n odd)*(14*n^4 -72*n^3 +226*n^2 -624*n +717)) / 192
 *            for n >= 2 (a(2)=0). Computed using 128‑bit arithmetic when
 *            available to avoid overflow even for very small seq_t types
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 30, u64 n <= 493, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(2) is the input offset; return 0 for n==2 and for any n<3 */
    if (n < 3) return 0;

    /* Use 128‑bit signed intermediate if the compiler supports it. */
#ifdef __SIZEOF_INT128__
    signed __int128 N = (signed __int128)n;
    signed __int128 num =
        N * N * N * N * N * N * N * N   /* n^8 */
        - (signed __int128)30 * N * N * N * N * N * N   /* n^6 */
        + (signed __int128)24 * N * N * N * N * N       /* n^5 */
        + (signed __int128)352 * N * N * N * N           /* n^4 */
        - (signed __int128)576 * N * N * N               /* n^3 */
        - (signed __int128)1280 * N * N                  /* n^2 */
        + (signed __int128)3360 * N
        - (signed __int128)1536;

    if (n & 1u) { /* odd n */
        num += (signed __int128)14 * N * N * N * N    /* 14*n^4 */
             - (signed __int128)72 * N * N * N       /* -72*n^3 */
             + (signed __int128)226 * N * N           /* +226*n^2 */
             - (signed __int128)624 * N              /* -624*n */
             + (signed __int128)717;                 /* +717 */
    }

    return (seq_t)(num / 192);
#else
    /* Fallback path: use seq_t arithmetic.  This may overflow for very
       small seq_t types, but the compiler is expected to provide __int128
       support for the intermediate calculations. */
    seq_t X = (seq_t)n;
    seq_t n2 = X * X;
    seq_t n3 = n2 * X;
    seq_t n4 = n2 * n2;
    seq_t n5 = n4 * X;
    seq_t n6 = n4 * n2;
    seq_t n8 = n4 * n4;

    seq_t positive = n8
                   + (seq_t)24 * n5
                   + (seq_t)352 * n4
                   + (seq_t)3360 * X;
    if (n & 1u) {
        positive += (seq_t)14 * n4
                  + (seq_t)226 * n2
                  + (seq_t)717;
    }

    seq_t negative = (seq_t)30 * n6
                   + (seq_t)576 * n3
                   + (seq_t)1280 * n2
                   + (seq_t)1536;
    if (n & 1u) {
        negative += (seq_t)72 * n3
                  + (seq_t)624 * X;
    }

    seq_t num = positive - negative;
    return num / (seq_t)192;
#endif
}
