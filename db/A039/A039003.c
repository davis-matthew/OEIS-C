/*
 * A039003  Numbers whose base-4 representation has the same number of 0's and
 *          2's.
 * https://oeis.org/A039003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth positive integer whose base‑4 representation
 *            contains the same number of digits 0 as digits 2. The function
 *            computes values by brute‑force enumeration
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9794 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static int equal_zero_two(seq_t num) {
    seq_t tmp = num;
    int zeros = 0;
    int twos  = 0;
    while (tmp > 0) {
        uint8_t d = (uint8_t)(tmp % 4);
        if (d == 0) zeros++;
        else if (d == 2) twos++;
        tmp /= 4;
    }
    return zeros == twos;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t cand = 1;
    while (count < n) {
        if (equal_zero_two(cand)) {
            count++;
            if (count == n) return cand;
        }
        cand++;
    }
    return cand; /* should never reach here */
}
