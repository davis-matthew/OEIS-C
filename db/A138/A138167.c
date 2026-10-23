/*
 * A138167  Numbers containing their length in ternary representation.
 * https://oeis.org/A138167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   contains_len_pattern
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth integer m whose base‑3 representation contains
   the base‑3 representation of its length (L) as a consecutive substring. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/contains_len_pattern.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 1;
    for (;;) {
        if (contains_len_pattern((seq_t)m)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
        ++m;
    }
}
