/*
 * A171113  a(n) is the Severi degree for curves of degree n and cogenus 3.
 * https://oeis.org/A171113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^6/2 - 27*n^5 + 9*n^4/2 + 423*n^3/2 - 229*n^2 -
 *            829*n/2 + 525 (for n > 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 32, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;

#if defined(__SIZEOF_INT128__) && !defined(__clang__) /* use 128‑bit intermediate when available */
    unsigned __int128 N  = n;
    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n3 * N;
    unsigned __int128 n5 = n4 * N;
    unsigned __int128 n6 = n5 * N;

    /* numerators of the terms having /2 are combined first */
    unsigned __int128 num =
          9ULL * n6                                 /* 9*n^6      */
        - 54ULL * n5                                /* -27*n^5*2  */
        + 9ULL * n4                                 /* +9*n^4     */
        + 423ULL * n3                               /* +423*n^3   */
        - 458ULL * n2                               /* -229*n^2*2 */
        - 829ULL * N                                /* -829*n/2*2 */
        + 1050ULL;                                 /* +525*2     */

    return (seq_t)(num >> 1);                       /* divide by 2 */
#else
    /* Fallback: compute using seq_t directly.  Works for values that fit
       into the underlying seq_t type (e.g. 64‑bit). */
    seq_t sn  = n;
    seq_t n2  = sn * sn;
    seq_t n3  = n2 * sn;
    seq_t n4  = n3 * sn;
    seq_t n5  = n4 * sn;
    seq_t n6  = n5 * sn;

    seq_t num =
          9 * n6              /* 9*n^6      */
        - 54 * n5             /* -27*n^5*2  */
        + 9 * n4              /* +9*n^4     */
        + 423 * n3            /* +423*n^3   */
        - 458 * n2            /* -229*n^2*2 */
        - 829 * sn            /* -829*n/2*2 */
        + 1050;               /* +525*2     */

    return num / 2;           /* final division */
#endif
}
