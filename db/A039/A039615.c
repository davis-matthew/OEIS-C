/*
 * A039615  Numbers whose base-12 representation has the same nonzero number
 *          of 2's and 4's.
 * https://oeis.org/A039615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer whose base‑12 representation contains
 *            the same non‑zero number of digits 2 and 4
 * Range:     u8 n <= 4, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t candidate = 1;
    idx_t found = 0;
    while (1) {
        seq_t t = candidate;
        int cnt2 = 0;
        int cnt4 = 0;
        /* count digits 2 and 4 in base‑12 */
        while (t > 0) {
            int digit = (int)(t % 12);
            if (digit == 2) ++cnt2;
            else if (digit == 4) ++cnt4;
            t = t / 12;
        }
        if (cnt2 == cnt4 && cnt2 != 0) {
            ++found;
            if (found == n)
                return candidate;
        }
        candidate = candidate + 1;
    }
}
