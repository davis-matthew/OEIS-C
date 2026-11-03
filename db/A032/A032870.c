/*
 * A032870  Numbers whose base-7 representation Sum_{i=0..m} d(i)*7^i has d(m)
 *          >= d(m-1) <= d(m-2) >= ...
 * https://oeis.org/A032870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032870
   Numbers whose base‑7 representation
   d(m) >= d(m-1) <= d(m-2) >= d(m-3) ...  (alternating inequalities)
   The function searches integers in order and returns the n‑th such number. */

#include <stdbool.h>

static bool passes_pattern(seq_t val) {
    /* Extract base‑7 digits into a small array */
    int d[32];
    int len = 0;
    seq_t v = val;
    while (v > (seq_t)0) {
        d[len++] = (int)(v % (seq_t)7);
        v /= (seq_t)7;
    }
    if (len == 0) return true;  /* zero not used, but keep safe */

    /* Check alternating inequalities from most significant to least */
    for (idx_t i = (idx_t)(len - 1); i > 0; --i) {
        int cur  = d[(int)i];
        int prev = d[(int)(i - 1)];
        /* When (len-1-i) is even, require cur >= prev */
        if (((len - 1 - i) & 1) == 0) {
            if (cur < prev) return false;
        } else {                      /* odd, require cur <= prev */
            if (cur > prev) return false;
        }
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t cnt = 0;
    seq_t cand = 1;
    while (1) {
        if (passes_pattern(cand)) {
            ++cnt;
            if (cnt == n) return cand;
        }
        ++cand;
    }
}
