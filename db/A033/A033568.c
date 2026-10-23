/*
 * A033568  Second pentagonal numbers with odd index: a(n) = (2*n-1)*(3*n-1).
 * https://oeis.org/A033568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)*(3*n-1)
 * Range:     u8 n <= 6, u16 n <= 104, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A033568: a(n) = 6*n^2 - 5*n + 1  (all operations in integer arithmetic) */
seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate to avoid overflow for 64‑bit builds. */
    unsigned __int128 t = (unsigned __int128)n * n;   /* n^2   */
    t = t * 6;                                       /* 6*n^2 */
    t -= (unsigned __int128)5 * n;                   /* -5*n  */
    t += 1;                                          /* +1    */
    return (seq_t)t;
}
