/*
 * A104574  Sum of trapezoid weights of all Motzkin paths of length n.
 * https://oeis.org/A104574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 43, u128 n <= 82,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 891 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: 
 *    n*a(n) + (-4n+3)*a(n-1) + (n-5)*a(n-2) + 2*(4n-5)*a(n-3) + (-5n+14)*a(n-4)
 *          + (-4n+7)*a(n-5) + 3*(n-3)*a(n-6) = 0
 *    a(1)=0, a(2)=1, a(3)=3, a(4)=10, a(5)=28, a(6)=80
 *
 * This is a linear recurrence with polynomial coefficients.  
 * The implementation evaluates it iteratively, keeping the last
 * computed values in a static array so that calls in arbitrary
 * order remain efficient.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

seq_t a_wide(idx_t n)
{
    /* base cases */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)10;
    if (n == 5) return (seq_t)28;
    if (n == 6) return (seq_t)80;

    /* static cache */
    static seq_t *seq = NULL;
    static size_t last = 0;
    static size_t cap   = 0;

    /* grow storage if necessary */
    if (n + 1 > cap) {
        size_t new_cap = (n + 1) * 2;
        /* realloc may return NULL only on failure; ignore for simplicity */
        seq = mem_resize(seq_t, seq, new_cap);
        cap = new_cap;
    }

    /* initialise base values once */
    if (last < 6) {
        seq[1] = (seq_t)0;
        seq[2] = (seq_t)1;
        seq[3] = (seq_t)3;
        seq[4] = (seq_t)10;
        seq[5] = (seq_t)28;
        seq[6] = (seq_t)80;
        last = 6;
    }

    /* compute forward up to requested n */
    for (idx_t i = last + 1; i <= n; ++i) {
        idx_t i_idx = i; /* local copy to avoid repeated promotion */

        /* positive contributions (coefficients that were negative in the eqn) */
        seq_t pos = seq[i_idx - 1] * (seq_t)(4 * i_idx - 3);
        pos = pos + seq[i_idx - 4] * (seq_t)(5 * i_idx - 14);
        pos = pos + seq[i_idx - 5] * (seq_t)(4 * i_idx - 7);

        /* negative contributions (coefficients that were positive) */
        seq_t neg = seq[i_idx - 2] * (seq_t)(i_idx - 5);
        neg = neg + seq[i_idx - 3] * (seq_t)(2 * (4 * i_idx - 5));
        neg = neg + seq[i_idx - 6] * (seq_t)(3 * (i_idx - 3));

        /* numerator: pos - neg  (pos > neg by construction) */
        seq_t num = pos - neg;

        /* divide by i to obtain a_wide(i) */
        seq[i_idx] = num / (seq_t)i_idx;
    }

    last = n;
    return seq[n];
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
