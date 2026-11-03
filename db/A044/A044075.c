/*
 * A044075  Numbers k such that the string 3,2 occurs in the base-4
 *          representation of k but not of k-1.
 * https://oeis.org/A044075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 3976, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9665 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A044075: numbers k such that the base‑4 representation
   contains the substring 3,2 and k−1 does not.  The function uses a
   simple brute‑force scan over successive integers. */
#include <stdbool.h>
#include <stdint.h>

static inline bool has32_uint64(uint64_t x) {
    uint8_t prev = 0;
    bool first = true;
    while (x > 0) {
        uint8_t d = x & 3U;        /* x % 4 */
        x >>= 2;                  /* x /= 4 */
        if (!first) {
            if (prev == 2 && d == 3) return true;
        } else {
            first = false;
        }
        prev = d;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 1;
    while (1) {
        if (has32_uint64(k) && !has32_uint64(k - 1)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
