/*
 * A064289  Height of n-th term in Recamán's sequence A005132.
 * https://oeis.org/A064289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 321, u16 n <= 9572, u32 n <= 9572, u64 n <= 9572,
 *            u128 n <= 9572, big any n
 * Helpers:   contains_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Height of the n‑th term in Recamán's sequence A005132.  
   The height is the number of addition steps minus the number of  
   subtraction steps performed up to index n.  (See OEIS A064289.) */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/contains_seq.h"

/* Grow a dynamic array of seq_t objects.  The function is called with
   the pointer to the array, its current capacity, and the desired
   capacity; realloc returns a pointer that is cast to seq_t*. */
static void grow_seq_t_array(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = *cap ? *cap : 8;
        while (new_cap < needed)
            new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* static state ----------------------------------------------------- */
    static seq_t *height = NULL;      /* height of each computed index   */
    static size_t height_cap = 0;
    static size_t max_computed = 0;   /* largest n computed so far      */

    static seq_t *seen = NULL;        /* all seen Recamán values        */
    static size_t seen_cap = 0, seen_cnt = 0;

    static seq_t cur_val = 0;         /* a(n-1) for the current step   */
    static seq_t cur_height = 0;      /* height after a(n-1)           */

    /* one‑time initialization ---------------------------------------- */
    if (!height) {
        grow_seq_t_array(&height, &height_cap, 8);
        height[0] = 0;
        max_computed = 0;

        grow_seq_t_array(&seen, &seen_cap, 8);
        seen[0] = 0;            /* a(0) is seen */
        seen_cnt = 1;
    }

    /* answer immediately if we already have it --------------------- */
    if (n <= max_computed)
        return height[(size_t)n];

    /* extend the computation up to n --------------------------------- */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Try subtraction first (Recamán's rule). */
        bool use_minus = false;
        seq_t minus_candidate = 0;

        if (cur_val >= (seq_t)i) {           /* no underflow */
            minus_candidate = cur_val - (seq_t)i;
            if (!contains_seq(seen, seen_cnt, minus_candidate))
                use_minus = true;
        }

        if (use_minus) {
            cur_val = minus_candidate;
            cur_height = cur_height - 1;     /* subtraction lowers height */
            grow_seq_t_array(&seen, &seen_cap, seen_cnt + 1);
            seen[seen_cnt++] = cur_val;
        } else {
            seq_t plus_candidate = cur_val + (seq_t)i;
            cur_val = plus_candidate;
            cur_height = cur_height + 1;     /* addition raises height */
            grow_seq_t_array(&seen, &seen_cap, seen_cnt + 1);
            seen[seen_cnt++] = cur_val;
        }

        /* store the computed height for index i */
        grow_seq_t_array(&height, &height_cap, (size_t)i + 1);
        height[(size_t)i] = cur_height;
    }

    max_computed = n;
    return height[(size_t)n];
}
