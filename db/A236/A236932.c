/*
 * A236932  Number of integer solutions to a^2 + b^2 + c^2 + 7*d^2 = n.
 * https://oeis.org/A236932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 7676, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A236932
   Number of integer solutions to a^2 + b^2 + c^2 + 7*d^2 = n.
   Offset is 0: a(0) = 1.  The function is valid for any n ≥ 0. */

/* Author: ChatGPT (adapted to required build environment) */

#ifdef __cplusplus
#  include <vector>
#endif
#include <stdlib.h>   /* For C: calloc, free; for C++: used only if needed */

seq_t a(idx_t n)
{
    /* Handle trivial case n < 0 is not expected; base case n = 0 is handled
       by the general algorithm. */

    /* Allocate arrays for r2 (solutions to a^2 + b^2 = k) and r3
       (solutions to a^2 + b^2 + c^2 = k).  In C++ we use std::vector
       which constructs seq_t objects correctly; in plain C we fall back
       on calloc. */
#ifdef __cplusplus
    std::vector<seq_t> r2((size_t)n + 1);
    std::vector<seq_t> r3((size_t)n + 1);
    seq_t *pr2 = r2.data();
    seq_t *pr3 = r3.data();
#else
    seq_t *pr2 = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *pr3 = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!pr2 || !pr3) {
        free(pr2); free(pr3);
        return 0;   /* Out of memory – return 0 as a safe fallback */
    }
#endif

    /* ------------------------------------------------------------------
       Compute r2[k] = number of integer solutions to a^2 + b^2 = k
       (counting ordered pairs (a,b) and sign variations). */
    for (idx_t i = 0; ; ++i) {
        idx_t i2 = i * i;
        if (i2 > n) break;
        seq_t vi = (i == 0) ? (seq_t)1 : (seq_t)2;
        for (idx_t j = 0; ; ++j) {
            idx_t j2 = j * j;
            idx_t sum = i2 + j2;
            if (sum > n) break;
            seq_t vj = (j == 0) ? (seq_t)1 : (seq_t)2;
            pr2[sum] += vi * vj;
        }
    }

    /* ------------------------------------------------------------------
       Compute r3[k] = number of integer solutions to a^2 + b^2 + c^2 = k.
       We convolve r2 with the values of a. */
    for (idx_t a = 0; ; ++a) {
        idx_t a2 = a * a;
        if (a2 > n) break;
        seq_t va = (a == 0) ? (seq_t)1 : (seq_t)2;
        idx_t limit = n - a2;
        for (idx_t k = 0; k <= limit; ++k) {
            if (pr2[k] == 0) continue;
            pr3[a2 + k] += pr2[k] * va;
        }
    }

    /* ------------------------------------------------------------------
       Sum over all possible d (≥ 0) such that 7*d^2 ≤ n.  For each d,
       multiply the number of (a,b,c) solutions by the sign multiplicity
       of d. */
    seq_t result = 0;
    for (idx_t d = 0; ; ++d) {
        idx_t d2 = d * d;
        if (7 * d2 > n) break;
        seq_t vd = (d == 0) ? (seq_t)1 : (seq_t)2;
        idx_t m = n - 7 * d2;
        result += pr3[m] * vd;
    }

    /* ------------------------------------------------------------------
       Free temporary memory in C; vectors free automatically in C++. */
#ifndef __cplusplus
    free(pr2);
    free(pr3);
#endif

    return result;
}
