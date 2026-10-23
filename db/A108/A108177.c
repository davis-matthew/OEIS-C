/*
 * A108177  Integers of the form 2^(4n-1) or 2^(4n), n>0 and their immediate
 *          neighbors, together with -1, 0 and 1.
 * https://oeis.org/A108177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = -1, a(1) = 0, a(2) = 1. For n ≥ 3: let m = (n – 3)/6 + 1
 *            and r = (n – 3) % 6. If r∈{0,1,2}: a(n) = 2^(4m‑1) + (r‑1). If
 *            r∈{3,4,5}: a(n) = 2^(4m) + (r‑3)
 * Range:     i8 n <= 9, i16 n <= 21, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   pow2_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2_exp.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)-1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    idx_t k = n - 3;
    idx_t m = k / 6 + 1;
    idx_t r = k % 6;

    seq_t p1 = pow2_exp((idx_t)(4 * m - 1));  /* 2^(4m-1) */
    seq_t p2 = pow2_exp((idx_t)(4 * m));      /* 2^(4m) */

    switch (r) {
        case 0: return p1 - (seq_t)1;            /* 2^(4m-1) - 1 */
        case 1: return p1;                      /* 2^(4m-1)     */
        case 2: return p1 + (seq_t)1;            /* 2^(4m-1) + 1 */
        case 3: return p2 - (seq_t)1;            /* 2^(4m)   - 1 */
        case 4: return p2;                      /* 2^(4m)       */
        case 5: return p2 + (seq_t)1;            /* 2^(4m)   + 1 */
    }
    return (seq_t)0;  /* unreachable */
}
