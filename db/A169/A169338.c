/*
 * A169338  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^30 = I.
 * https://oeis.org/A169338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 37 * 36^(n-1) for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_from_str("1");

    /* cached constants 36 and 37 (initialised once) */
    static seq_t c36, c37;
    static int init = 0;
    if (!init) {
        c36 = seq_from_str("36");
        c37 = seq_from_str("37");
        init = 1;
    }

    /* compute 36^(n-1) */
    seq_t pow = ipow_idx(c36, n - 1);

    /* multiply by 37 */
    seq_t res = seq_mul(c37, pow);

    return res;
}
