/*
 * A103138  Second column of inverse of Delannoy triangle.
 * https://oeis.org/A103138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   2*n*a(n)+(13*n-20)*a(n-1)+(8*n-27)*a(n-2)+(n-5)*a(n-3)=0 Initial
 *            values: a(0)=0; a(1)=1; a(2)=-3 Computes the nth term of OEIS
 *            sequence A103138 (second column of the inverse Delannoy
 *            triangle)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 28, i128 n <= 53,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 195 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
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
    static seq_t *memo = NULL;
    static idx_t memo_len = 0;          /* number of computed indices (memo_len = highest_index + 1) */

    if (n >= memo_len) {                /* need to compute up to n */
        idx_t old_len = memo_len;
        idx_t new_len = (n + 1 > 3) ? n + 1 : 3;   /* at least enough for indices 0..2 */
        memo_len = new_len;
        memo = mem_resize(seq_t, memo, new_len);

        if (old_len == 0) {             /* first allocation: set initial values */
            memo[0] = 0;
            memo[1] = 1;
            memo[2] = -3;
            old_len = 3;
        }

        /* compute missing terms */
        for (idx_t i = old_len; i <= n; ++i) {
            seq_t n_seq = (seq_t)i;
            seq_t t0 = memo[i-1];
            seq_t t1 = memo[i-2];
            seq_t t2 = memo[i-3];

            seq_t rhs = -(seq_t)13 * n_seq * t0;   /* -(13*n)*t0 */
            rhs += (seq_t)20 * t0;                 /* +20*t0 */
            rhs -= (seq_t)8 * n_seq * t1;          /* -(8*n)*t1 */
            rhs += (seq_t)27 * t1;                 /* +27*t1 */
            rhs -= (seq_t)n_seq * t2;              /* -(n)*t2 */
            rhs += (seq_t)5 * t2;                  /* +5*t2 */

            memo[i] = rhs / ((seq_t)2 * n_seq);    /* a_wide(i) = rhs / (2*n) */
        }
    }
    return memo[n];
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
