/*
 * A043869  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 2 mod 10.
 * https://oeis.org/A043869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑4 representation has
 *            (number of runs) ≡ 2 (mod 10)
 * Range:     u8 n <= 54, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1;; ++m) {
        idx_t tmp = m;
        int runs = 0;
        int last = -1;
        while (tmp > 0) {
            int d = tmp & 3;          /* tmp mod 4 */
            if (d != last) {          /* new run */
                ++runs;
                last = d;
            }
            tmp >>= 2;                /* tmp /= 4 */
        }
        if (runs % 10 == 2) {
            if (++count == n)
                return (seq_t)m;
        }
    }
}
