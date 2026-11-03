/*
 * A039093  Numbers whose base-9 representation has the same number of 2's and
 *          5's.
 * https://oeis.org/A039093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9684 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A039093: Numbers whose base‑9 representation has the same number
   of 2's and 5's.
   Implementation scans integers increasing from 0 until the desired
   candidate is found.  The function runs in O(m) time where m is the
   value of the n‑th term (≤ ≈ 10⁶ for n ≤ 10 000). */

#include <stdbool.h>

/* Helper: true if v's base‑9 digits contain an equal number of 2 and 5 */
static inline bool has_equal_25(seq_t v) {
    unsigned cnt2 = 0, cnt5 = 0;
    if (v == (seq_t)0) {
        /* 0 has no 2's and no 5's -> equal */
        return true;
    }
    while (v > (seq_t)0) {
        seq_t d = v % (seq_t)9;
        if (d == (seq_t)2) ++cnt2;
        else if (d == (seq_t)5) ++cnt5;
        v /= (seq_t)9;
    }
    return cnt2 == cnt5;
}

/* External entry point requested by the build system */
seq_t a(idx_t n) {
    idx_t found = 0;          /* how many valid numbers we have seen */
    seq_t cur   = (seq_t)0;   /* current integer to test */

    /* Scan upwards until the n-th valid number is found. */
    while (1) {
        if (has_equal_25(cur)) {
            ++found;
            if (found == n) {
                return cur;
            }
        }
        ++cur;
    }
}
