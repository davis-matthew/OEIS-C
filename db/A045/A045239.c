/*
 * A045239  Numbers whose base-5 representation contains exactly one 1 and
 *          three 3's.
 * https://oeis.org/A045239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045239: Numbers whose base-5 representation contains exactly one 1 and three 3's. */
/* The algorithm scans consecutive integers, checking each one's base‑5 digits for the required pattern. */
/* It stops when the n‑th qualifying number is found. */

#include <stdbool.h>

static inline bool has_property(seq_t x) {
    unsigned ones = 0, threes = 0;
    while (x) {
        unsigned d = (unsigned)(x % 5);
        if (d == 1) {
            if (++ones > 1) return false;
        } else if (d == 3) {
            if (++threes > 3) return false;
        }
        x /= 5;
    }
    return (ones == 1) && (threes == 3);
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t cand   = 0;
    while (count < (seq_t)n) {
        ++cand;
        if (has_property(cand)) {
            ++count;
        }
    }
    return cand;
}
