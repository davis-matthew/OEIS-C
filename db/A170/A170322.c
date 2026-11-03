/*
 * A170322  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=25*24^(n-1) for n>=1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_i32.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline seq_t seq_one(void) { return (seq_t)1; }

seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();

    /* compute 24^(n-1) and multiply by 25 */
    seq_t pow24 = ipow_i32((seq_t)24, (int)(n - 1));
    return seq_mul(pow24, (seq_t)25);
}
