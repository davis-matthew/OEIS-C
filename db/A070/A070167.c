/*
 * A070167  a(n) is the smallest starting value that produces a Collatz
 *          sequence in which n occurs.
 * https://oeis.org/A070167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest starting value that produces a Collatz sequence in which n occurs.
   The function simply tests all possible starts from 1 up to n and returns
   the first one that reaches n.  It works for any seq_t/idx_t types
   that satisfy the given constraints. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_even.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* offset is 1 in OEIS */
    for (idx_t s = 1; s <= n; ++s) {
        seq_t v = (seq_t)s;
        while (true) {
            if (v == (seq_t)n) return (seq_t)s;
            if (v == 1) break;
            if (is_even(v)) v >>= 1;
            else           v = v * 3 + 1;
        }
    }
    return (seq_t)0;                          /* should never happen */
}
