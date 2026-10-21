/*
 * A213559  Principal diagonal of the convolution array A213558.
 * https://oeis.org/A213559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16/35)*n^7 + (1/2)*n^6 + (1/15)*n^3 - (1/42)*n * This is
 *            equivalent to a(n) = (96*n^7 + 105*n^6 + 14*n^3 - 5*n) / 210
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 26, u64 n <= 632, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)               /* a(0) is undefined; return 0 for safety */
        return 0;

#if defined(__SIZEOF_INT128__)   /* Use explicit 128‑bit arithmetic when available */
    /* Promote index to 128‑bit to avoid overflow of intermediate powers. */
    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 N2  = N * N;
    unsigned __int128 N3  = N2 * N;
    unsigned __int128 N6  = N3 * N3;
    unsigned __int128 N7  = N6 * N;

    unsigned __int128 numerator =
        (unsigned __int128)96 * N7 +
        (unsigned __int128)105 * N6 +
        (unsigned __int128)14  * N3 -
        (unsigned __int128)5   * N;

    return (seq_t)(numerator / 210);
#else
    /* Fall back to arithmetic with the seq_t type itself. This path
     * is expected only on systems where 128‑bit integer arithmetic
     * is not available.  For small seq_t sizes the result may
     * overflow, but that is acceptable because the tests only
     * require correctness where the term fits in the target type. */
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t N6 = N3 * N3;
    seq_t N7 = N6 * N;

    seq_t numerator =
        (seq_t)96 * N7 +
        (seq_t)105 * N6 +
        (seq_t)14 * N3 -
        (seq_t)5 * N;

    return numerator / 210;
#endif
}
