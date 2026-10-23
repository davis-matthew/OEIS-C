/*
 * A137832  Limiting sequence when we start with the positive integers
 *          (A000027) and delete in step n >= 1 the last digit in the term at
 *          position n + a(n).
 * https://oeis.org/A137832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A137832 – Limiting sequence when we start with the positive integers
    and delete in step n ≥ 1 the last digit in the term at position
    n + a(n).  The code simulates the process, storing the values of
    a(n) as they are computed.  It caches results so that any call
    with arbitrary n (≥1) returns the correct value.  The algorithm
    is O(n) for the first n values, with a small constant factor. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Grow an array of seq_t elements, copying them with the assignment
   operator (appropriate for both POD and arbitrary‑precision types). */
static void *grow_array(void *old, size_t old_len, size_t new_cap)
{
    size_t elem_size = sizeof(seq_t);
    seq_t *new_arr = (seq_t *)malloc(new_cap * elem_size);
    if (!new_arr) abort();
    if (old && old_len > 0) {
        seq_t *src = (seq_t *)old;
        seq_t *dst = new_arr;
        for (size_t i = 0; i < old_len; ++i)
            dst[i] = src[i];
        free(old);
    }
    return new_arr;
}

/* Mutable sequence of current terms. */
static seq_t *list = NULL;
static idx_t  list_len = 0;
static size_t list_cap = 0;
static idx_t  next_nat = 1;          /* Next natural number to append */

/* Cached a(n) values. */
static seq_t *a_vals = NULL;
static idx_t  a_len  = 0;
static size_t a_cap  = 0;

/* Ensure the list has at least 'need' elements (1‑based). */
static void ensure_list_len(idx_t need)
{
    if (list_len >= need) return;
    if ((size_t)need > list_cap) {
        size_t new_cap = list_cap ? list_cap * 2 : 64;
        if (new_cap < (size_t)need) new_cap = (size_t)need;
        list = (seq_t *)grow_array(list, list_len, new_cap);
        list_cap = new_cap;
    }
    while (list_len < need) {
        list[list_len++] = (seq_t)next_nat++;
    }
}

/* Ensure the a_vals array has capacity for 'need' elements (0‑based). */
static void ensure_a_cap(idx_t need)
{
    if ((size_t)need <= a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 64;
    if (new_cap < (size_t)need) new_cap = (size_t)need;
    a_vals = (seq_t *)grow_array(a_vals, a_len, new_cap);
    a_cap = new_cap;
}

/* Return a(n) for n ≥ 1.  Extra argument for n = 0 is unused. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1 */

    if (n <= a_len) return a_vals[n - 1];

    for (idx_t k = a_len + 1; k <= n; ++k) {
        ensure_list_len(k);
        seq_t ak = list[k - 1];          /* value before deletion */
        ensure_a_cap(a_len + 1);
        a_vals[a_len++] = ak;

        idx_t del_idx = k + (idx_t)ak;  /* 1‑based position to delete */
        ensure_list_len(del_idx);

        size_t del0 = (size_t)(del_idx - 1);   /* 0‑based index */
        seq_t val  = list[del0];
        seq_t new_val = val / 10;             /* truncate last digit */

        if (new_val == 0) {                  /* remove element */
            size_t j;
            for (j = del0; j + 1 < list_len; ++j)
                list[j] = list[j + 1];
            --list_len;
        } else {
            list[del0] = new_val;            /* replace with truncated value */
        }
    }
    return a_vals[n - 1];
}
