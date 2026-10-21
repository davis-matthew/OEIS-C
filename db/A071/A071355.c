/*
 * A071355  a(n) = 2*n^2 + 11*n + 12.
 * https://oeis.org/A071355
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + 11*n + 12 Offset: -1 (a(-1) = 3)
 * Range:     u8 n <= 8, u16 n <= 178, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Use 128‑bit arithmetic to avoid intermediate overflow */
    __int128 t = (__int128) n * n;   /* n^2 */
    t *= 2;                          /* 2*n^2 */
    t += (__int128) 11 * n;          /* + 11*n */
    t += 12;                         /* + 12 */
    return (seq_t) t;
}
