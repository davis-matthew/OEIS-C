/*
 * A072823  Numbers that are not the sum of two powers of 2.
 * https://oeis.org/A072823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 220, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9857 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A072823 – Numbers that are not the sum of two powers of 2.
   a(1) = 1; for n > 1, a(n) is the n‑th integer ≥ 2 whose binary
   representation contains at least three 1‑bits. */
#include <stdint.h>
#include <stdbool.h>

static inline bool has_at_least_three_set_bits(idx_t x)
{
    /* Use the GCC/Clang builtin popcount for 64‑bit values. */
    unsigned pc = __builtin_popcountll((uint64_t)x);
    return pc >= 3;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;      /* 1 is the first term by definition. */

    idx_t count = 1;          /* we have already counted the value 1. */
    for (idx_t x = 2; ; ++x) {
        if (has_at_least_three_set_bits(x)) {
            count++;
            if (count == n)
                return (seq_t)x;
        }
    }
}
