/*
 * A033966  Trajectory of 1 under map n->19n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, a(n) = a(n‑1)/2 if a(n‑1) is even,
 *            otherwise a(n) = 19 · a(n‑1) + 1. This defines the trajectory of
 *            1 under the map n → 19 n + 1 (for odd n) and n → n/2 (for even
 *            n)
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t cur = 1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        if ((cur & 1) == 0)        /* even */
            cur = cur >> 1;
        else                       /* odd */
            cur = (seq_t)19 * cur + 1;
    }
    return cur;
}
