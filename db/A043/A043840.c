/*
 * A043840  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 3 mod 6.
 * https://oeis.org/A043840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   runs_base4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043840: numbers n such that the number of runs in the base‑4 representation of n is congruent to 3 modulo 6. */
/* The implementation computes the n‑th term by a direct search. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/runs_base4.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for out‑of‑bounds calls */
    seq_t val = (seq_t)1;
    idx_t seen = 0;
    while (seen < n) {
        if (runs_base4(val) % 6 == 3) {
            ++seen;
            if (seen == n) return val;
        }
        ++val;
    }
    return (seq_t)0;                      /* unreachable; added to satisfy all paths */
}
