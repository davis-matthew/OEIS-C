/*
 * A039323  Numbers whose base-7 representation has the same nonzero number of
 *          3's and 4's.
 * https://oeis.org/A039323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Linear search for base‑7 numbers that contain the same non‑zero
 *            number of digits 3 and 4. The function returns the n‑th such
 *            number (1‑based)
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined for 0, but avoid UB */
    idx_t found = 0;
    seq_t x = 1;
    while (found < n) {
        int c3 = 0, c4 = 0;
        seq_t v = x;
        while (v > 0) {
            int d = (int)(v % 7);
            if (d == 3) c3++;
            else if (d == 4) c4++;
            v = v / 7;
        }
        if (c3 == c4 && c3 > 0) {
            ++found;
            if (found == n) return x;
        }
        x = x + 1;
    }
    return 0;   /* unreachable */
}
