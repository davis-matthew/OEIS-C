/*
 * A182146  The number of simple labeled graphs on n nodes with no components
 *          of size 1 or 2.
 * https://oeis.org/A182146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2^{n*(n-1)/2} - b(n)
 * where b(n) counts labelled graphs whose every component has size >= 3.
 * b(0) = 1, and for n >= 1: 
 *     b(n) = sum_{k=3}^{n} binom(n-1, k-1) * c(k) * b(n-k)
 * with c(k) the number of connected labelled graphs on k vertices:
 *     c(1) = 1, and for n >= 2:
 *         c(n) = 2^{n*(n-1)/2} - sum_{k=1}^{n-1} binom(n-1, k-1) * c(k) *
 *                2^{(n-k)*(n-k-1)/2}
 *
 * The implementation builds auxiliary tables up to the maximum n requested
 * and evaluates the above recurrence.  It uses only integer arithmetic
 * and works for any seq_t that supports +, -, *, / and assignment.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t *pow2_arr = NULL;
static seq_t *t_arr    = NULL;
static seq_t *c_arr    = NULL;
static seq_t *b_arr    = NULL;
static idx_t  cur_size = 0;   /* number of stored entries (index 0..cur_size-1) */

/* extend tables to be able to answer a(n) for all n <= limit */
static void ensure_up_to(idx_t limit)
{
    if (limit + 1 <= cur_size)
        return;                      /* already computed */

    idx_t new_size = limit + 1;

    /* allocate new arrays */
    seq_t *new_pow2 = (seq_t*)calloc(new_size, sizeof(seq_t));
    seq_t *new_t    = (seq_t*)calloc(new_size, sizeof(seq_t));
    seq_t *new_c    = (seq_t*)calloc(new_size, sizeof(seq_t));
    seq_t *new_b    = (seq_t*)calloc(new_size, sizeof(seq_t));

    /* copy existing data */
    if (cur_size > 0) {
        memcpy(new_pow2, pow2_arr, cur_size * sizeof(seq_t));
        memcpy(new_t,    t_arr,    cur_size * sizeof(seq_t));
        memcpy(new_c,    c_arr,    cur_size * sizeof(seq_t));
        memcpy(new_b,    b_arr,    cur_size * sizeof(seq_t));
    }

    /* free old arrays */
    free(pow2_arr);
    free(t_arr);
    free(c_arr);
    free(b_arr);

    /* reassign */
    pow2_arr = new_pow2;
    t_arr    = new_t;
    c_arr    = new_c;
    b_arr    = new_b;

    /* initialise base cases if this is the first run */
    if (cur_size == 0) {
        pow2_arr[0] = 1;   /* 2^0 */
        t_arr[0]    = 1;   /* 2^{0} */
        c_arr[0]    = 0;   /* unused */
        b_arr[0]    = 1;   /* empty graph */
    }

    /* fill entries from cur_size up to limit */
    idx_t start = cur_size;
    if (start == 0) start = 1;   /* we already have index 0 */

    /* powers of two: pow2[i] = 2^i */
    for (idx_t i = start; i <= limit; ++i)
        pow2_arr[i] = pow2_arr[i-1] + pow2_arr[i-1];   /* multiply by 2 */

    /* total number of labelled graphs on i vertices */
    for (idx_t i = start; i <= limit; ++i)
        t_arr[i] = t_arr[i-1] * pow2_arr[i-1];        /* t[i] = t[i-1] * 2^{i-1} */

    /* number of connected labelled graphs c[i] */
    for (idx_t i = start; i <= limit; ++i) {
        if (i == 1) {
            c_arr[i] = 1;
        } else if (i >= 2) {
            seq_t sum  = 0;
            seq_t binom = 1;  /* binom(n-1,k-1) starts at k=1 */
            for (idx_t k = 1; k < i; ++k) {
                seq_t term = binom * c_arr[k];
                term *= t_arr[i - k];
                sum += term;

                /* update binom for next k */
                seq_t num = (seq_t)(i - k);
                binom = binom * num / (seq_t)k;
            }
            c_arr[i] = t_arr[i] - sum;
        }
    }

    /* b[i] = number of graphs where every component has size >= 3 */
    for (idx_t i = start; i <= limit; ++i) {
        seq_t sum  = 0;
        seq_t binom = 1;  /* binom(n-1,k-1) starts at k=1 */
        for (idx_t k = 1; k < i; ++k) {
            if (k >= 3) {
                seq_t term = binom * c_arr[k];
                term *= b_arr[i - k];
                sum += term;
            }
            seq_t num = (seq_t)(i - k);
            binom = binom * num / (seq_t)k;
        }
        if (i >= 3)
            sum += c_arr[i];   /* term for k = i */
        b_arr[i] = sum;
    }

    cur_size = new_size;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;            /* undefined for n < 1 */

    ensure_up_to(n);
    return t_arr[n] - b_arr[n];
}
