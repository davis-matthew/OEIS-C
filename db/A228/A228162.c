/*
 * A228162  Number of bangbangs (!!) in shell substitution when starting with
 *          : '!!' and : "!!" '!!'.
 * https://oeis.org/A228162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n=0, 2 for n=1, 2 for n=2, 3 for n=3, 5 for n=4,
 *            and for n >= 5: a(n) = 4*p(n) + 2*q(n) + 1, p(5) = 2, q(5) = 4,
 *            p(n+1) = 3*p(n)^2 + 2*p(n) + p(n)*q(n), q(n+1) = 2*p(n)^2 +
 *            2*p(n)*q(n) + 2*q(n)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *p_arr = NULL;
static seq_t *q_arr = NULL;
static idx_t p_max = 4;   /* largest n for which p_arr/q_arr are initialized */

seq_t a(idx_t n)
{
    /* Initial explicit values */
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)2;
        case 2: return (seq_t)2;
        case 3: return (seq_t)3;
        case 4: return (seq_t)5;
        default: break;   /* fall through for n >= 5 */
    }

    /* Lazy initialization of p/q arrays for n >= 5 */
    if (p_max < 5) {
        /* Allocate space for indices 0..5 */
        p_arr = (seq_t*)malloc(6 * sizeof(seq_t));
        q_arr = (seq_t*)malloc(6 * sizeof(seq_t));
        if (!p_arr || !q_arr) abort();

        /* Base values for recurrence */
        p_arr[5] = (seq_t)2;
        q_arr[5] = (seq_t)4;
        p_max = 5;
    }

    /* Extend the arrays up to the requested n */
    if (n > p_max) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(p_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        p_arr = tmp;
        tmp = (seq_t*)realloc(q_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        q_arr = tmp;

        for (idx_t i = p_max + 1; i <= n; ++i) {
            seq_t p_prev = p_arr[i - 1];
            seq_t q_prev = q_arr[i - 1];
            seq_t p_sq   = p_prev * p_prev;

            /* Recurrence for p and q */
            seq_t p_i = 3 * p_sq + 2 * p_prev + p_prev * q_prev;
            seq_t q_i = 2 * p_sq + 2 * p_prev * q_prev + 2 * q_prev;

            p_arr[i] = p_i;
            q_arr[i] = q_i;
        }
        p_max = n;
    }

    /* Compute a(n) from p(n) and q(n) */
    return 4 * p_arr[n] + 2 * q_arr[n] + (seq_t)1;
}
