/*
 * A032612  Concatenation of n and n+7.
 * https://oeis.org/A032612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(n, n+7) = n * 10^digits(n+7) + (n+7)
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/num_digits0.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Ensure offset 1: return 0 for n==0 (unused but safe) */
    if (n == 0)
        return (seq_t)0;

    /* Compute n+7 as uint64_t (fits in idx_t range) */
    uint64_t m = (uint64_t)n + 7;

    /* Number of decimal digits of m */
    unsigned d = num_digits0(m);

    /* 10^d as seq_t */
    seq_t pow10_val = pow10((idx_t)d);

    /* n * 10^d as seq_t */
    seq_t prod = seq_mul((seq_t)n, pow10_val);

    /* Final result */
    return prod + (seq_t)m;
}
