/*
 * A108931  a(2n) = -A106328(n), a(2n+1) = A054488(n).
 * https://oeis.org/A108931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 25, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A108931
    a(0) = 0
    a(2n) = -A106328(n)   (n ≥ 1)
    a(2n+1) = A054488(n)

    A106328 satisfies  u(1)=3 , u(2)=18 ,  u(n)=6·u(n-1)-u(n-2)  (n≥3)
    A054488 satisfies  v(0)=1 , v(1)=8  ,  v(n)=6·v(n-1)-v(n-2)  (n≥2)
*/

#include <stdlib.h>

static seq_t *even_seq = NULL;      /* stores u(i+1)  – i.e. u(1) at even_seq[0] */
static size_t even_len = 0;
static size_t even_cap = 0;

static seq_t *odd_seq = NULL;       /* stores v(i)    – v(0) at odd_seq[0]   */
static size_t odd_len  = 0;
static size_t odd_cap  = 0;

static void ensure_even(size_t idx)
{
    while (even_len <= idx) {
        /* allocate or grow storage */
        if (even_cap < even_len + 1) {
            size_t new_cap = even_cap ? even_cap * 2 : 4;
            if (new_cap < even_len + 1) new_cap = even_len + 1;
            seq_t *tmp = (seq_t *)realloc(even_seq, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);             /* abort on out‑of‑memory */
            even_seq = tmp;
            even_cap = new_cap;
        }

        if (even_len == 0) {               /* u(1) = 3 */
            even_seq[0] = (seq_t)3;
            even_len   = 1;
        } else if (even_len == 1) {        /* u(2) = 18 */
            even_seq[1] = (seq_t)18;
            even_len   = 2;
        } else {                           /* recurrence */
            even_seq[even_len] =
                (seq_t)6 * even_seq[even_len - 1] - even_seq[even_len - 2];
            ++even_len;
        }
    }
}

static void ensure_odd(size_t idx)
{
    while (odd_len <= idx) {
        /* allocate or grow storage */
        if (odd_cap < odd_len + 1) {
            size_t new_cap = odd_cap ? odd_cap * 2 : 4;
            if (new_cap < odd_len + 1) new_cap = odd_len + 1;
            seq_t *tmp = (seq_t *)realloc(odd_seq, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            odd_seq = tmp;
            odd_cap = new_cap;
        }

        if (odd_len == 0) {               /* v(0) = 1 */
            odd_seq[0] = (seq_t)1;
            odd_len   = 1;
        } else if (odd_len == 1) {        /* v(1) = 8 */
            odd_seq[1] = (seq_t)8;
            odd_len   = 2;
        } else {                           /* recurrence */
            odd_seq[odd_len] =
                (seq_t)6 * odd_seq[odd_len - 1] - odd_seq[odd_len - 2];
            ++odd_len;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    if ((n & 1) == 0) {                 /* even index: a(2m) = -u(m) */
        size_t m   = (size_t)(n >> 1);   /* m = n/2  (m ≥ 1) */
        size_t idx = m - 1;              /* u(1) stored at even_seq[0] */
        ensure_even(idx);
        return -even_seq[idx];
    } else {                             /* odd index: a(2m+1) = v(m) */
        size_t idx = (size_t)((n - 1) >> 1);   /* m = (n-1)/2 */
        ensure_odd(idx);
        return odd_seq[idx];
    }
}
