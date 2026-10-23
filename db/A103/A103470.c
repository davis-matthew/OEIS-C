/*
 * A103470  Number of polyominoes consisting of 4 regular unit n-gons.
 * https://oeis.org/A103470
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 722, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A103470
 *  Number of polyominoes consisting of 4 regular unit n-gons.
 *
 *  The sequence satisfies a linear recurrence derived from its
 *  rational generating function:
 *
 *      G(x) = -x^3*(x^17 -2x^16 +3x^15 -4x^14 +4x^13 -4x^12 +4x^11
 *                 -2x^10 +2x^9 -2x^8 +4x^7 -x^6 +x^5 +3x^4 -3x^3
 *                 +3x^2 -x +3) /
 *          ((x-1)^3*(x+1)*(x^2-x+1)^2*(x^2+1)*(x^2+x+1)^2*(x^4-x^2+1))
 *
 *  Let Q(x) = Σ_{i=0}^{18} q_i x^i be the product of the denominator
 *  factors.  Then for n ≥ 3
 *
 *          q_0 a[n] + Σ_{i=1}^{18} q_i a[n-i] = r[n]
 *
 *  where r[n] = -coeff_{n-3} of the numerator polynomial for
 *  n ≤ 20 and r[n] = 0 otherwise.  The coefficients are:
 *
 *          q = [-1,  2, -2,  2, -2,  2, -1,  0,  0,  0,  0,  0,
 *                 1, -2,  2, -2,  2, -2,  1]
 *
 *          r = [0, 0, 0, -3,  1, -3,  3, -3, -1,  1,
 *                -4,  2, -2,  2, -4,  4, -4,  4, -3,  2, -1]
 *
 *  The recurrence can be rewritten as
 *
 *          a[n] = Σ_{i=1}^{18} q_i a[n-i] - r[n]
 *
 *  because q_0 = -1.  Values for n < 3 are taken as 0, so a[0]=a[1]=a[2]=0.
 *
 *  The implementation memoises the sequence so that calls in
 *  arbitrary order are efficient.  It runs in O(n·18) time per
 *  new value and uses only integer arithmetic.
 */

#include <stdlib.h>
#include <string.h>

/* coefficients of Q(x) */
static const int Q[19] = {
    -1,  /* q0  */
     2,  /* q1  */
    -2,  /* q2  */
     2,  /* q3  */
    -2,  /* q4  */
     2,  /* q5  */
    -1,  /* q6  */
     0,  /* q7  */
     0,  /* q8  */
     0,  /* q9  */
     0,  /* q10 */
     0,  /* q11 */
     1,  /* q12 */
    -2,  /* q13 */
     2,  /* q14 */
    -2,  /* q15 */
     2,  /* q16 */
    -2,  /* q17 */
     1   /* q18 */
};

/* inhomogeneous term r[n] for 0 ≤ n ≤ 20, 0 otherwise */
static const int R[21] = {
     0, /* r0 */
     0, /* r1 */
     0, /* r2 */
    -3, /* r3 */
     1, /* r4 */
    -3, /* r5 */
     3, /* r6 */
    -3, /* r7 */
    -1, /* r8 */
     1, /* r9 */
    -4, /* r10 */
     2, /* r11 */
    -2, /* r12 */
     2, /* r13 */
    -4, /* r14 */
     4, /* r15 */
    -4, /* r16 */
     4, /* r17 */
    -3, /* r18 */
     2, /* r19 */
    -1  /* r20 */
};

seq_t a(idx_t n)
{
    /* a[n] is defined only for n ≥ 3.  For n < 3 return 0. */
    if (n < 3)
        return 0;

    /* static memoisation state */
    static idx_t   max_n   = 0;       /* largest n computed so far */
    static size_t  cap     = 0;       /* allocated capacity of table */
    static seq_t * table   = NULL;    /* memoisation table */

    /* Allocate or grow the table as necessary. */
    if (table == NULL || cap <= n) {
        size_t new_cap = (cap < 3 ? 3 : cap);
        while (new_cap <= n)
            new_cap <<= 1;              /* exponential growth */

        seq_t *new_table = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (new_table == NULL)
            return 0;                 /* out of memory → safe return */

        if (table != NULL) {
            memcpy(new_table, table, sizeof(seq_t) * cap);
            free(table);
        }
        table = new_table;
        cap   = new_cap;
    }

    /* Compute any missing entries up to n. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t rest = 0;                 /* Σ_{k=1}^{18} q_k a[i-k]  */
        for (int k = 1; k <= 18; ++k) {
            if (i >= (idx_t)k)
                rest += (seq_t)Q[k] * table[i - k];
        }

        seq_t rhs = 0;                   /* r[i] for i ≤ 20, else 0 */
        if (i <= 20)
            rhs = (seq_t)R[i];

        table[i] = rest - rhs;           /* a[i] = rest - r[i]   */
    }
    max_n = n;

    return table[n];
}
