/*
 * A168116  a(n) = n*(n^8+1)/2.
 * https://oeis.org/A168116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^8+1)/2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 149,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* No overflow for 64‑bit seq_t: use 128‑bit intermediate if available. */
    if (n == 0) return 0;

#if defined(__SIZEOF_INT128__)
    unsigned __int128 N   = (unsigned long long)n;
    unsigned __int128 pow = 1;
    for (int i = 0; i < 8; ++i) pow *= N;          /* N^8 */
    unsigned __int128 res = N * (pow + 1) / 2;      /* (N^9+N)/2  */
    return (seq_t)res;
#else
    /* Fallback path – works when seq_t is 128‑bit or arbitrary precision. */
    seq_t pow = 1;
    for (int i = 0; i < 8; ++i) pow *= (seq_t)n;     /* n^8 may wrap, but if result fits seq_t it will be correct. */
    if ((n & 1) == 0)            /* n even: (n/2)*(n^8+1) */
        return ((seq_t)n / 2) * (pow + (seq_t)1);
    else                          /* n odd: n*((n^8+1)/2) */
        return (seq_t)n * ((pow + (seq_t)1) / 2);
#endif
}
