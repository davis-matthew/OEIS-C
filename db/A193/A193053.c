/*
 * A193053  a(n) = (14*n*(n+3) + (2*n-5)*(-1)^n + 21)/16.
 * https://oeis.org/A193053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*n*(n+3) + (2*n-5)*(-1)^n + 21)/16
 * Range:     u8 n <= 15, u16 n <= 272, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
#if defined(__SIZEOF_INT128__)
    /* Use 128‑bit signed arithmetic when available. */
    __int128 N   = (__int128)n;                     /* index as signed 128‑bit     */
    __int128 res = 14 * N * (N + 3) + 21;            /* 14*n*(n+3)+21               */
    if ((n & 1) == 0)                              /* n even → add (2*n‑5)        */
        res += 2 * N - 5;
    else                                            /* n odd  → subtract (2*n‑5)   */
        res -= 2 * N - 5;
    res >>= 4;                                      /* divide by 16 (2⁴)           */
    return (seq_t)res;
#else
    /* Fallback to 64‑bit signed arithmetic. */
    long long N   = (long long)n;
    long long res = 14 * N * (N + 3) + 21;
    if ((n & 1) == 0)
        res += 2 * N - 5;
    else
        res -= 2 * N - 5;
    res /= 16;
    return (seq_t)res;
#endif
}
