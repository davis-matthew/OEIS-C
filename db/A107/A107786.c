/*
 * A107786  a(n) = |b(n)| where b(n) = -b(n-1) + 6*b(n-3) with b(0)=0, b(1)=1,
 *          b(2)=1.
 * https://oeis.org/A107786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 67, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: b(n) = -b(n-1) + 6*b(n-3),  b(0)=0, b(1)=1, b(2)=1,
   a(n) = |b(n)|,  a(0)=0, a(1)=1, a(2)=1. */

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    seq_t val;          /* absolute value of b(k) */
    unsigned char sign; /* 0 if non‑negative, 1 if negative */
} abt_t;

static abt_t *b_arr      = NULL;      /* holds b(k) and its sign          */
static idx_t  last_computed = 0;      /* highest index that has been computed */
static idx_t  array_size    = 0;      /* current capacity (number of elements) */
static bool   init_done     = false;

/* Ensure that the dynamic array has room for indices 0..n inclusive. */
static void ensure_capacity(idx_t n)
{
    if (!init_done) {
        /* Allocate enough space for at least the first three terms. */
        size_t sz = (n > 2) ? (size_t)(n + 1) : 3;
        b_arr = (abt_t *)calloc(sz, sizeof(abt_t));
        if (!b_arr) exit(1);
        b_arr[0].val = (seq_t)0;  b_arr[0].sign = 0;
        b_arr[1].val = (seq_t)1;  b_arr[1].sign = 0;
        b_arr[2].val = (seq_t)1;  b_arr[2].sign = 0;
        last_computed = 2;
        array_size    = sz;
        init_done     = true;
    } else if (array_size <= n) {
        size_t new_size = (size_t)(n + 1);
        abt_t *tmp = (abt_t *)realloc(b_arr, new_size * sizeof(abt_t));
        if (!tmp) exit(1);
        b_arr = tmp;
        for (idx_t i = array_size; i <= n; ++i) {
            b_arr[i].val  = (seq_t)0;
            b_arr[i].sign = 0;
        }
        array_size = new_size;
    }
}

seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* If the value has already been computed, return it directly. */
    if (n <= last_computed) {
        return b_arr[n].val;
    }

    /* Compute from the next unused index up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* term1 = -b(i-1)  (handle zero specially) */
        abt_t term1;
        if (b_arr[i - 1].val == (seq_t)0) {
            term1.val  = (seq_t)0;
            term1.sign = 0;
        } else {
            term1.val  = b_arr[i - 1].val;
            term1.sign = b_arr[i - 1].sign ^ 1;   /* change sign */
        }

        /* term2 = 6 * b(i-3) */
        abt_t term2;
        term2.val  = (seq_t)6 * b_arr[i - 3].val;
        term2.sign = b_arr[i - 3].sign;

        /* Add term1 and term2 with sign handling */
        abt_t res;
        if (term1.sign == term2.sign) {
            res.sign = term1.sign;
            res.val  = term1.val + term2.val;
        } else {
            if (term1.val >= term2.val) {
                res.sign = term1.sign;
                res.val  = term1.val - term2.val;
            } else {
                res.sign = term2.sign;
                res.val  = term2.val - term1.val;
            }
        }
        if (res.val == (seq_t)0) {
            res.sign = 0;
        }
        b_arr[i] = res;
    }
    last_computed = n;
    return b_arr[n].val;
}
