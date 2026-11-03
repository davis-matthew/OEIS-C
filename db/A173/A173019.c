/*
 * A173019  a(n) is the value of row n in triangle A083093 seen as ternary
 *          number.
 * https://oeis.org/A173019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum_{k=0..n} (C(n,k) mod 3) * 3^k (value of row n of
 *            Pascal’s triangle mod 3 seen as a base‑3 number)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 724 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

static seq_t *seq_arr = NULL;           /* array of computed values */
static size_t seq_cap  = 0;              /* capacity of seq_arr    */
static size_t seq_cnt  = 0;              /* number of values stored */

static unsigned char *row   = NULL;     /* digits of the current row (base‑3 digits) */
static size_t row_len  = 0;              /* length of that row = n+1          */

seq_t a(idx_t n)
{
    /* Initialization on first call */
    if (seq_arr == NULL) {
        seq_cap = 16;
        seq_arr = seq_alloc_array(seq_cap);   /* allocates & default‑constructs */

/* seq_cnt == 0 before first insertion      */
        seq_cnt = 0;

        /* first row n = 0 : [1] */
        row_len = 1;
        row = (unsigned char*)malloc(row_len);
        row[0] = 1;

        seq_arr[0] = (seq_t)1;
        seq_cnt = 1;   /* we now have a(0) */
    }

    /* Grow the values array if necessary */
    while ((size_t)n >= seq_cap) {
        size_t new_cap = seq_cap * 2;
        if (new_cap == 0) new_cap = 16;
        grow_seq_array(&seq_arr, &seq_cap, new_cap);
    }

    /* Compute rows up to n, if not already present */
    idx_t curr;
    for (curr = (idx_t)(seq_cnt - 1); curr < n; ++curr) {
        /* Build next row */
        size_t next_len = row_len + 1;
        unsigned char *next = (unsigned char*)malloc(next_len);
        next[0] = 1;
        for (size_t k = 1; k < row_len; ++k) {
            unsigned char sum = row[k-1] + row[k];
            if (sum >= 3) sum -= 3;
            next[k] = sum;
        }
        next[row_len] = 1;          /* last digit */

        /* Compute the base‑3 value of this row */
        seq_t value = 0;
        for (size_t k = 0; k < next_len; ++k) {
            value = value * 3 + (seq_t)next[k];
        }

        /* Store the value */
        seq_arr[seq_cnt] = value;
        ++seq_cnt;

        /* Prepare for next iteration */
        free(row);
        row = next;
        row_len = next_len;
    }

    /* Finally return the required value */
    return seq_arr[n];
}
