/*
 * A224778  One half of the even numbers that are the sum of four nonzero
 *          squares.
 * https://oeis.org/A224778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A224778: one half of the even numbers that are the sum of four positive squares */
/* The algorithm generates even numbers E sequentially and checks whether E can be
   expressed as a sum of four positive squares.  It does this efficiently by
   pre‑computing all sums of two positive squares up to the current maximum E
   and then testing each possible pair (a,b) for a representation.  Results are
   memoised so that successive calls can continue from where the previous call
   left off. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* seq_t and idx_t are supplied by the build system; we use them directly. */

static seq_t *a_values = NULL;   /* cache of computed terms */
static size_t a_len   = 0;        /* number of terms stored */
static size_t a_cap   = 0;        /* allocated capacity of a_values */

static unsigned char *sum_sq = NULL; /* table of sums of two positive squares */
static idx_t sum_cap = 0;            /* largest index covered by sum_sq */

static idx_t next_even = 4;          /* next even number to test */

/* Resize sum_sq so it can index up to new_cap (inclusive). */
static void ensure_sum_cap(idx_t new_cap)
{
    if (new_cap <= sum_cap)
        return;

    size_t new_bytes = (new_cap + 1) * sizeof(unsigned char);
    unsigned char *new_arr = (unsigned char *)malloc(new_bytes);
    if (!new_arr)
        abort();                   /* out of memory */

    memset(new_arr, 0, new_bytes);

    if (sum_sq) {
        memcpy(new_arr, sum_sq, (size_t)sum_cap + 1);
        free(sum_sq);
    }

    /* fill new entries */
    for (idx_t a = 1; a * a <= new_cap; ++a) {
        idx_t a2 = a * a;
        for (idx_t b = 1; a2 + b * b <= new_cap; ++b) {
            idx_t s = a2 + b * b;
            new_arr[s] = 1;
        }
    }

    sum_sq  = new_arr;
    sum_cap = new_cap;
}

/* Test whether an even integer e can be represented as the sum of
   four positive squares. */
static bool is_sum4_pos(idx_t e)
{
    /* e >= 4 and even */
    for (idx_t a = 1; a * a <= e - 3; ++a) {
        idx_t a2 = a * a;
        idx_t max_b = e - a2 - 2;      /* need at least 2 remaining */
        for (idx_t b = 1; b * b <= max_b; ++b) {
            idx_t rem = e - a2 - b * b;
            if (rem >= 2 && sum_sq[rem])
                return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                    /* undefined for offset 1 */

    /* Generate terms until we have at least n terms. */
    while (a_len < n) {
        idx_t e = next_even;
        if (e > sum_cap)
            ensure_sum_cap(e);

        if (is_sum4_pos(e)) {
            if (a_len + 1 > a_cap)
                grow_seq(&a_values, &a_cap, a_len + 1);
            a_values[a_len++] = (seq_t)(e / 2);
        }
        next_even += 2;
    }

    return a_values[n - 1];
}
