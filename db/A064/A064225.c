/*
 * A064225  a(n) = (9*n^2 + 5*n + 2)/2.
 * https://oeis.org/A064225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^2 + 5*n + 2)/2
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
#if defined(__SIZEOF_INT128__) || defined(__GNUC__) || defined(__clang__)
    unsigned __int128 x = n;
    unsigned __int128 tmp = 9 * x * x + 5 * x + 2;
    return (seq_t)(tmp >> 1); /* divide by 2 */
#else
    /* fallback for compilers without 128‑bit integer support */
    seq_t nn = (seq_t)n * (seq_t)n;
    return ((seq_t)9 * nn + (seq_t)5 * (seq_t)n + (seq_t)2) / 2;
#endif
}
