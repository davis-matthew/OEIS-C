/*
 * A039232  Numbers whose base-12 representation has the same number of 2's
 *          and 5's.
 * https://oeis.org/A039232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer whose base‑12 representation contains *
 *            the same number of digits 2 and 5 (computed by brute‑force
 *            search)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool good(seq_t x) {
    idx_t c2 = 0, c5 = 0;
    seq_t two = (seq_t)2, five = (seq_t)5;
    while (x > 0) {
        seq_t d = x % 12;
        if (d == two) c2++;
        else if (d == five) c5++;
        x /= 12;
    }
    return c2 == c5;
}

seq_t a(idx_t n) {
    seq_t cand = 0;
    idx_t cnt = 0;
    while (cnt < n) {
        if (good(cand)) {
            cnt++;
            if (cnt == n) return cand;
        }
        cand++;
    }
    return 0;  /* unreachable */
}
