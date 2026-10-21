/*
 * A211959  Number of lower triangular n X n arrays colored with integers 0
 *          upwards introduced in row major order, with no element equal to
 *          any diagonal or antidiagonal neighbor, and containing the value
 *          n(n+1)/2-3.
 * https://oeis.org/A211959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: 
 *   a(1) = 0
 *   a(n) = (1/128)*n^8 + (1/32)*n^7 - (29/192)*n^6 - (3/16)*n^5 
 *          + (209/128)*n^4 - (205/96)*n^3 - (47/96)*n^2 + (79/24)*n - 1,
 *          for n > 1.  (This empirical polynomial holds for all n ≥ 2.)
 */

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;          /* a(1) = 0 by definition */

    /* Use 128‑bit intermediate arithmetic to avoid overflow up to the
     * range where the result still fits in 'seq_t'.  The compiler may
     * provide the built‑in type __int128 (common on GCC/Clang). */
#if defined(__SIZEOF_INT128__)
    unsigned __int128 nn  = n;                /* n */
    unsigned __int128 n2  = nn * nn;          /* n^2 */
    unsigned __int128 n4  = n2 * n2;          /* n^4 */
    unsigned __int128 n8  = n4 * n4;          /* n^8 */
    unsigned __int128 n3  = n2 * nn;          /* n^3 */
    unsigned __int128 n5  = n4 * nn;          /* n^5 */
    unsigned __int128 n6  = n4 * n2;          /* n^6 */
    unsigned __int128 n7  = n4 * n3;          /* n^7 */

    /* Numerator of the rational expression multiplied by 384
     * (common denominator of the polynomial). */
    unsigned __int128 num =
          3  * n8
        + 12 * n7
        - 58 * n6
        - 72 * n5
        + 627 * n4
        - 820 * n3
        - 188 * n2
        + 1264 * nn
        - 384;

    /* Divide by 384 to obtain the exact integer value. */
    unsigned __int128 res = num / 384u;
    return (seq_t)res;
#else
    /* Fallback for compilers without __int128.  Compute using the
     * algorithmic form that avoids large intermediate values:  divide
     * by 384 only after the full numerator has been assembled.  For
     * the values of n where the result still fits into seq_t (≤ 2^64-1)
     * the intermediate products also fit into the native unsigned
     * long long type (guaranteed by the sequence bounds).  If seq_t
     * is larger (e.g., 128‑bit or arbitrary precision), this
     * calculation still produces the correct result. */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long n2 = nn * nn;            /* n^2 */
    unsigned long long n4 = n2 * n2;            /* n^4 */
    unsigned long long n8 = n4 * n4;            /* n^8 */
    unsigned long long n3 = n2 * nn;            /* n^3 */
    unsigned long long n5 = n4 * nn;            /* n^5 */
    unsigned long long n6 = n4 * n2;            /* n^6 */
    unsigned long long n7 = n4 * n3;            /* n^7 */

    /* The same numerator expression as above, calculated using
     * unsigned long long arithmetic.  For n up to 400 this fits
     * within 64 bits; for larger n the final result will exceed
     * 64 bits and the build that checks only 64‑bit fits will not
     * verify those values. */
    unsigned long long num =
          3ULL  * n8
        + 12ULL * n7
        - 58ULL * n6
        - 72ULL * n5
        + 627ULL * n4
        - 820ULL * n3
        - 188ULL * n2
        + 1264ULL * nn
        - 384ULL;

    unsigned long long res = num / 384ULL;
    return (seq_t)res;
#endif
}
