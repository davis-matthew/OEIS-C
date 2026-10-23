/*
 * A044857  Positive integers having no fewer base-16 runs of even length than
 *          odd.
 * https://oeis.org/A044857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044857: Positive integers whose base‑16 representation has
   no fewer runs of even length than runs of odd length. */

#include <stdbool.h>

static inline bool satisfies(seq_t v) {
    if (v == 0) return false;          /* 0 has one odd run, fails */
    /* extract base‑16 digits (least significant first) */
    uint8_t digits[256];                /* supports up to 256 hex digits */
    int len = 0;
    seq_t tmp = v;
    do {
        digits[len++] = (uint8_t)(tmp & 0xFULL);
        tmp >>= 4;
    } while (tmp != 0);

    int evens = 0, odds = 0;
    int run_len = 1;
    /* scan from most significant to least significant digit */
    for (int i = len - 2; i >= 0; --i) {
        if (digits[i] == digits[i + 1]) {
            run_len++;
        } else {
            if (run_len & 1) odds++; else evens++;
            run_len = 1;
        }
    }
    /* last run */
    if (run_len & 1) odds++; else evens++;

    return evens >= odds;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset is 1, but guard against 0 */
    idx_t seen = 0;
    seq_t cand = 1;
    for (;;) {
        if (satisfies(cand)) {
            seen++;
            if (seen == n) return cand;
        }
        cand++;
    }
}
