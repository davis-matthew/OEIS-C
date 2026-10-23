/*
 * A043782  Numbers k such that number of runs in base 3 representation of k
 *          is congruent to 2 mod 5.
 * https://oeis.org/A043782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: integers whose base‑3 representation has a number of runs ≡ 2 (mod 5).
   The function scans increasing integers and returns the n‑th such integer
   (offset 1). */

seq_t a(idx_t n) {
    idx_t found = 0;
    idx_t k = 1;
    while (found < n) {
        idx_t tmp = k;
        int runs = 0;
        int prev = -1;
        while (tmp > 0) {
            int digit = (int)(tmp % 3);
            if (digit != prev) {
                ++runs;
                prev = digit;
            }
            tmp /= 3;
        }
        if (runs % 5 == 2) {
            ++found;
            if (found == n) {
                return (seq_t)k;
            }
        }
        ++k;
    }
    /* Should never be reached for valid n */
    return (seq_t)0;
}
