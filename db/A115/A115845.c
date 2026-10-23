/*
 * A115845  Numbers n such that there is no bit position where the binary
 *          expansions of n and 8n are both 1.
 * https://oeis.org/A115845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth non‑negative integer m (offset 1) such that (m &
 *            (m << 3)) == 0. That is, m has no bit position where both m and
 *            8*m have a 1. a(1) = 0, a(2) = 1, …
 * Range:     u8 n <= 75, u16 n <= 3549, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for offset 1, but return 0 safely */
        return (seq_t)0;

    idx_t found = 0;
    idx_t m = 0;

    for (;; ++m) {
        seq_t x = (seq_t)m;
        if ((x & (x << 3)) == 0) {
            ++found;
            if (found == n)
                return x;
        }
    }
}
