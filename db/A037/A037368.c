/*
 * A037368  Numbers whose base-7 and base-9 expansions have no digits in
 *          common.
 * https://oeis.org/A037368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 829 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: find the nth positive integer whose base-7 and base-9 expansions have no digits in common */
#include <stdint.h>
#include <stdbool.h>

/* Check whether an unsigned 64‑bit integer has no common digits in its base‑7
   and base‑9 representations. */
static bool has_disjoint_base_digits(uint64_t x)
{
    uint16_t mask7 = 0;
    uint16_t mask9 = 0;
    uint64_t t = x;

    /* Mask for digits in base 7 (digits 0–6) */
    while (t) {
        mask7 |= 1u << (t % 7);
        t /= 7;
    }

    t = x;
    /* Mask for digits in base 9 (digits 0–8) */
    while (t) {
        mask9 |= 1u << (t % 9);
        t /= 9;
    }

    return (mask7 & mask9) == 0;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used but safe */

    idx_t found = 0;
    uint64_t m = 1;

    while (1) {
        if (has_disjoint_base_digits(m)) {
            ++found;
            if (found == n) {
                return (seq_t)m;
            }
        }
        ++m;
    }
}
