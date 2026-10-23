/*
 * A187921  Positions k of addition steps in Recamán's sequence where
 *          A005132(k-1)<=k.
 * https://oeis.org/A187921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 5771, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A187921: positions k of addition steps in Recamán's sequence where A005132(k‑1) ≤ k */

/* The function iteratively generates Recamán's sequence and records
 * the indices at which an addition occurs and the previous value is
 * not greater than the index.  Results are cached between calls. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static state */
static bool          initialized      = false;
static idx_t         cur_step         = 0;   /* last computed step (Recamán index) */
static seq_t         cur_value        = 0;   /* value at cur_step */

static size_t        pos_cap          = 0;   /* capacity of positions array */
static size_t        pos_len          = 0;   /* number of stored positions */
static idx_t        *pos_list         = NULL;/* array of qualifying positions */

static size_t        vis_cap          = 0;   /* capacity of visited array */
static size_t        vis_len          = 0;   /* number of stored values */
static seq_t        *vis_list         = NULL;/* array of visited Recamán values */

/* Append a value to the visited array, growing it if necessary. */
static void vis_append(seq_t v)
{
    if (vis_len == vis_cap) {
        vis_cap = vis_cap ? vis_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(vis_list, vis_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        vis_list = tmp;
    }
    vis_list[vis_len++] = v;
}

/* Check whether a value is already in the visited array. */
static bool vis_contains(seq_t v)
{
    for (size_t i = 0; i < vis_len; ++i)
        if (vis_list[i] == v)
            return true;
    return false;
}

/* Append a qualifying position to the positions array. */
static void pos_append(idx_t v)
{
    if (pos_len == pos_cap) {
        pos_cap = pos_cap ? pos_cap * 2 : 16;
        idx_t *tmp = (idx_t *)realloc(pos_list, pos_cap * sizeof(idx_t));
        if (!tmp) exit(1);
        pos_list = tmp;
    }
    pos_list[pos_len++] = v;
}

/* Main function implementing OEIS sequence A187921. */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* Allocate and initialise visited array with the first value 0. */
        vis_cap = 16;
        vis_list = (seq_t *)malloc(vis_cap * sizeof(seq_t));
        if (!vis_list) exit(1);
        vis_append((seq_t)0);
        vis_len = 1;

        /* Allocate positions array. */
        pos_cap = 16;
        pos_list = (idx_t *)malloc(pos_cap * sizeof(idx_t));
        if (!pos_list) exit(1);
        pos_len = 0;

        cur_step  = 0;
        cur_value = 0;
        initialized = true;
    }

    /* Generate steps until we have at least n qualifying positions. */
    while (pos_len < n) {
        idx_t k  = cur_step + 1;          /* next step index (1‑based) */
        seq_t prev = cur_value;           /* value before this step */

        bool perform_subtract = false;
        if (prev > (seq_t)k) {
            seq_t cand = prev - (seq_t)k;
            if (!vis_contains(cand))
                perform_subtract = true;
        }

        if (perform_subtract) {
            cur_value = prev - (seq_t)k;
        } else {
            cur_value = prev + (seq_t)k;
            if (prev <= (seq_t)k)          /* qualifies for the sequence */
                pos_append(k);
        }

        vis_append(cur_value);
        cur_step = k;
    }

    return pos_list[n - 1];
}
