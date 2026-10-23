/*
 * A022939  Unique increasing sequence satisfying a(n) = a(n-2) + c(n-2);
 *          where c( ) is complement of a( ).
 * https://oeis.org/A022939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n‑2) + c(n‑2), where c(k) is the k‑th positive integer
 *            not yet appearing in a. Initial values: a(1) = 1, a(2) = 2
 * Range:     u8 n <= 27, u16 n <= 487, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Grow a dynamic array of seq_t, allocating new elements with default construction.
   Works in both C and C++ (C++ uses new/delete, C uses realloc). */
static void grow_seq_t(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < need)
        new_cap <<= 1;

#ifdef __cplusplus
    /* C++: allocate, copy, and delete old array */
    seq_t *new_arr = new seq_t[new_cap];
    if (*arr) {
        for (size_t i = 0; i < *cap; ++i)
            new_arr[i] = (*arr)[i];
        delete [] *arr;
    }
    *arr = new_arr;
#else
    /* C: use realloc (cast to seq_t*) */
    seq_t *new_arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    if (!new_arr)
        abort(); /* allocation failed */
    *arr = new_arr;
#endif
    *cap = new_cap;
}

/* Grow a dynamic array of bool using realloc (cast to bool*). */
static void grow_bool_array(bool **arr, size_t *cap, size_t need)
{
    if (need <= *cap)
        return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < need)
        new_cap <<= 1;
    bool *new_arr = (bool*)realloc(*arr, new_cap * sizeof(bool));
    if (!new_arr)
        abort(); /* allocation failed */
    /* initialize new entries to false */
    memset(new_arr + *cap, 0, (new_cap - *cap) * sizeof(bool));
    *arr = new_arr;
    *cap = new_cap;
}

/* Global static cache */
static seq_t *a_seq = NULL;              /* a[0] unused, a[1].. */
static size_t a_cap = 0;
static size_t a_len = 0;                 /* next free index (a_len-1 is last valid) */

static seq_t *missing = NULL;            /* c(1) stored at missing[0] */
static size_t missing_cap = 0;
static size_t missing_len = 0;

static bool *present = NULL;             /* present[x] == true if x in a */
static size_t present_cap = 0;
static idx_t present_next = 1;           /* next integer candidate for missing */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (a_len == 0) {
        /* a[1] = 1, a[2] = 2 */
        grow_seq_t(&a_seq, &a_cap, 3);
        a_seq[1] = 1;
        a_seq[2] = 2;
        a_len = 3;   /* next free index */

        grow_bool_array(&present, &present_cap, 3); /* indices 0..2 */
        present[0] = false;
        present[1] = true;
        present[2] = true;
        present_next = 3;
    }

    /* Compute terms up to n if needed */
    for (idx_t idx = (idx_t)a_len; idx <= n; ++idx) {
        seq_t val;
        if (idx == 1) {
            val = 1;            /* already handled in init, but keep for safety */
        } else if (idx == 2) {
            val = 2;
        } else {
            /* Ensure sufficient c(k) are available */
            while (missing_len < idx - 2) {
                /* Find next missing integer */
                idx_t cand = present_next;
                grow_bool_array(&present, &present_cap, (size_t)cand + 1);
                while (present[(size_t)cand]) {
                    ++cand;
                    grow_bool_array(&present, &present_cap, (size_t)cand + 1);
                }
                /* Store missing number */
                grow_seq_t(&missing, &missing_cap, missing_len + 1);
                missing[missing_len++] = (seq_t)cand;
                present_next = cand + 1;
            }
            seq_t cn = missing[idx - 3];   /* c(idx-2) = missing[idx-3] */
            val = a_seq[idx - 2] + cn;
        }

        /* Store value */
        grow_seq_t(&a_seq, &a_cap, (size_t)idx + 1);
        a_seq[idx] = val;
        ++a_len;

        /* Mark value as present */
        size_t val_idx = (size_t)val;
        grow_bool_array(&present, &present_cap, val_idx + 1);
        present[val_idx] = true;
    }

    return a_seq[n];
}
