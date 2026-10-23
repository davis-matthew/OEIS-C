/*
 * A024185  Fourth elementary symmetric function of 3,4,...,n+5.
 * https://oeis.org/A024185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            n*(n+1)*(n+2)*(n+3)*(15*n^4+450*n^3+5165*n^2+26922*n+53848)/5760
 * Range:     u8 none, u16 n <= 4, u32 n <= 29, u64 n <= 534, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The calculation is performed in 128‑bit arithmetic when available
       to avoid overflow, otherwise it falls back to the exact formula
       using seq_t arithmetic. */
#if defined(__SIZEOF_INT128__)
    unsigned __int128 N   = n;       /*  n  */
    unsigned __int128 prod = N * (N + 1);
    prod *= (N + 2);
    prod *= (N + 3);               /* n(n+1)(n+2)(n+3) */

    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n3 * N;  /* n^2, n^3, n^4 */

    unsigned __int128 p = 15 * n4;          /* 15*n^4 */
    p += 450 * n3;             /* +450*n^3 */
    p += 5165 * n2;            /* +5165*n^2 */
    p += 26922 * N;            /* +26922*n */
    p += 53848;                /* +53848 */

    unsigned __int128 result = prod * p;
    result /= 5760;
    return (seq_t)result;
#else
    /* Fallback for compilers without built‑in 128‑bit integers.
       seq_t is assumed to support arbitrary precision or large enough
       for the intermediate results. */
    seq_t N   = (seq_t)n;
    seq_t prod = N * (N + 1);
    prod *= (N + 2);
    prod *= (N + 3);

    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;

    seq_t p = 15 * n4;
    p += 450 * n3;
    p += 5165 * n2;
    p += 26922 * N;
    p += 53848;

    seq_t result = prod * p;
    result /= 5760;
    return result;
#endif
}
