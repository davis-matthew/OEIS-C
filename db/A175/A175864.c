/*
 * A175864  Partial sums of ceiling(n^2/19).
 * https://oeis.org/A175864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n^3 + 3*n^2 + 61*n + 78)/114)
 * Range:     u8 n <= 23, u16 n <= 154, u32 n <= 6255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth partial sum of ceiling(k^2 / 19). */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* compute numerator in 128‑bit if available to avoid overflow */
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus) /* C compiler */
    __uint128_t num = ( (__uint128_t)N * (__uint128_t)N * (__uint128_t)N * 2 )
                    + ( (__uint128_t)N * (__uint128_t)N * 3 )
                    + ( (__uint128_t)N * 61 )
                    + 78;
    return (seq_t)(num / 114);
#else
    /* C++ compiler or no 128‑bit support – fall back to seq_t arithmetic */
    seq_t num = N * N * N * 2 + N * N * 3 + N * 61 + 78;
    return num / 114;
#endif
}
