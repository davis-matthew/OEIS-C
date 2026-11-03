/*
 * A101082  Numbers n such that binary representation contains bit strings
 *          "10" and "01" (possibly overlapping).
 * https://oeis.org/A101082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 219, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9310 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finds the nth integer whose binary representation contains
 * both substrings "10" and "01" (possibly overlapping).
 * The sequence starts with 5, 9, 10, 11, 13, ... as per OEIS A101082. */

#include <stdint.h>
#include <stdbool.h>

/* Returns true if x contains the bit strings "10" and "01". */
static bool has10and01(uint64_t x)
{
    bool has10 = false, has01 = false;
    int prev = x & 1;
    x >>= 1;
    while (x) {
        int curr = x & 1;
        if (prev == 1 && curr == 0) has10 = true;
        if (prev == 0 && curr == 1) has01 = true;
        if (has10 && has01) return true;
        prev = curr;
        x >>= 1;
    }
    return false;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t candidate = 5;          /* smallest number with both patterns */
    while (1) {
        if (has10and01(candidate)) {
            count++;
            if (count == n) return (seq_t)candidate;
        }
        candidate++;
    }
}
