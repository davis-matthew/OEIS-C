/*
 * A020662  Lexicographically earliest increasing sequence of positive numbers
 *          that contains no arithmetic progression of length 9.
 * https://oeis.org/A020662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th element of the lexicographically earliest
 *            increasing sequence of positive integers that contains no
 *            arithmetic progression of length 9. The implementation builds
 *            the sequence greedily: at each step it adds the smallest integer
 *            larger than the current maximum that does not create a...
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*--- storage for the produced sequence ----------------------------------*/
static seq_t *seq_arr  = NULL;   /* array of selected elements */
static size_t seq_cap  = 0;      /* allocated capacity */
static size_t seq_len  = 0;      /* number of elements stored */

/*** utility to grow the sequence array ***/
static void grow_seq(void)
{
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    seq_t *new_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!new_arr) abort();          /* out of memory */
    seq_arr = new_arr;
    seq_cap = new_cap;
}

/*--- bit set for fast membership test ---------------------------------*/
static uint8_t *visited   = NULL; /* bitmap: visited[v] == 1 if v present */
static size_t  vis_cap    = 0;    /* allocated bitmap size */
static size_t  vis_len    = 0;    /* logical length (max value + 1) */

/* ensure the bitmap can index up to 'needed' (inclusive) */
static void ensure_vis_cap(size_t needed)
{
    if (needed > vis_cap) {
        size_t cap = vis_cap ? vis_cap << 1 : 1024;
        while (cap < needed) cap <<= 1;
        uint8_t *new_vis = (uint8_t *)realloc(visited, cap);
        if (!new_vis) abort();          /* out of memory */
        /* initialise newly allocated area to zero */
        memset(new_vis + vis_cap, 0, cap - vis_cap);
        visited = new_vis;
        vis_cap = cap;
    }
    if (needed > vis_len) vis_len = needed;
}

/* member test: returns true iff value v is already in the set */
static inline bool visited_contains(size_t v)
{
    return v < vis_len && visited[v];
}

/*--- core function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined for n < 1 */

    /* extend the sequence up to index n */
    while (seq_len < (size_t)n) {
        /* candidate: start just after the current largest element */
        seq_t cand = (seq_len == 0) ? 1 : seq_arr[seq_len - 1] + (seq_t)1;

        /* find the smallest acceptable candidate */
        for (;;) {
            /* convert to size_t for bitmap operations */
            size_t cand_sz = (size_t)cand;
            bool bad = false;
            size_t max_d = (cand_sz > 1) ? (cand_sz - 1) / 8 : 0;

            for (size_t d = 1; d <= max_d && !bad; ++d) {
                bool all_present = true;
                /* check the 8 predecessors cand - k*d */
                for (int k = 1; k <= 8; ++k) {
                    size_t y = cand_sz - k * d;
                    if (!visited_contains(y)) {
                        all_present = false;
                        break;
                    }
                }
                if (all_present) bad = true;   /* would form a 9‑term AP */
            }
            if (!bad) break;                /* cand is acceptable */
            cand += (seq_t)1;                /* try next integer */
        }

        /* store the chosen candidate */
        if (seq_len == seq_cap) grow_seq();
        seq_arr[seq_len] = cand;
        seq_len++;

        /* update the bitmap */
        size_t cand_sz = (size_t)cand;
        ensure_vis_cap(cand_sz + 1);
        visited[cand_sz] = 1;
    }

    return seq_arr[n - 1];
}
