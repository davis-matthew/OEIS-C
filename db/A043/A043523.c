/*
 * A043523  Numbers having three 8's in base 10.
 * https://oeis.org/A043523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 53 terms through n = 9266 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers having exactly three 8's in base 10 (OEIS A043523)
   Computation: brute‑force search scanning integer values and counting
   digits. */
#include <stdint.h>
#include <stdbool.h>

/* Helper: test whether a 64‑bit integer contains exactly three digit 8's. */
static inline bool has_three_eights(uint64_t x)
{
    uint8_t cnt = 0;
    while (x) {
        uint8_t d = x % 10;
        if (d == 8) {
            ++cnt;
            if (cnt > 3)
                return false;
        }
        x /= 10;
    }
    return cnt == 3;
}

/* Returns the n‑th integer that contains exactly three digits 8 (n >= 1). */
seq_t a(idx_t n)
{
    idx_t seen = 0;
    for (uint64_t num = 888ULL; ; ++num) {
        if (has_three_eights(num)) {
            ++seen;
            if (seen == n)
                return (seq_t)num;
        }
    }
}
