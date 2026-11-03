/*
 * A170472  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (if n==0) otherwise a(n) = 31 * 30^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utility headers */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_u64_idx.h"

static inline seq_t seq_one(void) { return (seq_t)1; }

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return seq_one();          /* a(0) = 1 */
    }
    /* Compute 30^(n-1) using the provided power routine */
    seq_t pow30 = ipow_u64_idx(30ULL, n - 1);
    /* Multiply by 31 */
    return seq_mul(pow30, (seq_t)31);
}
