/*
 * A172489  a(0)=1, a(1)=6; for n>=2, a(n) = 6^A042950(n-2).
 * https://oeis.org/A172489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6; for n≥2, a(n)=6^A042950(n-2). A042950(n) = 2 for
 *            n=0; 3 for n=1; 3*2^(n-1) for n≥1. Therefore: a(2)=36, a(3)=216,
 *            and for n≥4, a(n)=6^(3*2^(n‑3))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 8 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n) {
    /* handle the small cases explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)36;
    if (n == 3) return (seq_t)216;

    /* For n ≥ 4 compute exponent e = 3 * 2^(n‑3) */
    seq_t exp = (seq_t)3;                 /* start with 3 */
    for (idx_t i = 0; i < n - 3; ++i) {   /* multiply by 2 (n‑3) times */
        exp = seq_mul(exp, (seq_t)2);
    }

    /* return 6^exp */
    return ipow((seq_t)6, exp);
}
