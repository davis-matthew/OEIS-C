/*
 * A058222  Tree of tournament sequences read across rows.
 * https://oeis.org/A058222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, tabf
 * Formula:   a(n) = value of node at index n in the tree where each node k
 *            has children k+1, k+2, … , 2k, read across rows
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for the sequence */
static seq_t *tree_cache = NULL;   /* array storing the values */
static size_t tree_len   = 0;      /* number of values in the cache */
static size_t tree_cap   = 0;      /* current allocated capacity */
static size_t process_idx = 0;     /* index of the next node to process */

/* Return the n-th term of the sequence (a(0)=1). */
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (tree_cache == NULL) {
        tree_cap = 64;
        tree_cache = (seq_t *)malloc(tree_cap * sizeof(seq_t));
        if (tree_cache == NULL) return 0;   /* allocation failure */
        tree_cache[0] = (seq_t)1;           /* root node */
        tree_len = 1;
        process_idx = 0;
    }

    /* Generate terms until we have at least n+1 values */
    while (tree_len <= (size_t)n) {
        seq_t parent = tree_cache[process_idx];
        seq_t max_child = parent * ((seq_t)2);     /* 2 * parent */

        for (seq_t child = parent + (seq_t)1; child <= max_child; ++child) {
            if (tree_len == tree_cap) {            /* grow the array */
                tree_cap *= 2;
                seq_t *new_cache = (seq_t *)realloc(tree_cache, tree_cap * sizeof(seq_t));
                if (new_cache == NULL) return 0;  /* allocation failure */
                tree_cache = new_cache;
            }
            tree_cache[tree_len++] = child;
        }
        ++process_idx;
    }

    return tree_cache[n];
}
