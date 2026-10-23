/*
 * A125338  Number of base 28 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of base 28 circular n‑digit numbers where
 * adjacent digits differ by at most 3.
 * Computation: a(0)=1, a(1)=28.
 * For n≥1, a(n) = trace(Aⁿ) where A is the 28×28 adjacency matrix with
 * A[i][j] = 1 if |i−j|≤3, otherwise 0 (including self‑loops).
 * We generate a(n) by iteratively multiplying the current power
 * matrix by A and accumulating the diagonal sums.  The matrix
 * is 28×28, banded (width 7), so each step costs about 5 k
 * additions – easily below the time limit for n≤10 000.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef unsigned char uint8_t;     /* placeholder – seq_t/idx_t are defined by the build */

/* Function prototype supplied by the build environment */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* Internal data structures                                         */
static bool          initialized = false;
static seq_t        *cache       = NULL;   /* sequence values */
static idx_t          cache_len   = 0;     /* number of stored terms */
static idx_t          cur_len     = 0;     /* length of current matrix power */
static seq_t          cur[28][28];        /* A^cur_len */
static seq_t          nxt[28][28];        /* temporary matrix */

static void init(void)
{
    /* identity matrix for A⁰ */
    for (idx_t i = 0; i < 28; ++i)
        for (idx_t j = 0; j < 28; ++j)
            cur[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* allocate cache for a(0) = 1 */
    cache       = (seq_t *)malloc(sizeof(seq_t) * 1);
    cache[0]    = (seq_t)1;
    cache_len   = 1;
    cur_len     = 0;        /* current power is A⁰ */
    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
seq_t a(idx_t n)
{
    if (!initialized) init();

    /* Extend cache and matrix powers up to required n */
    while (n >= cache_len) {
        /* Compute nxt = cur * A (banded) */
        for (idx_t i = 0; i < 28; ++i) {
            for (idx_t j = 0; j < 28; ++j) {
                seq_t sum = 0;
                idx_t lo = (j >= 3) ? (j - 3) : 0;
                idx_t hi = (j + 3 <= 27) ? (j + 3) : 27;
                for (idx_t k = lo; k <= hi; ++k) {
                    sum += cur[i][k];
                }
                nxt[i][j] = sum;
            }
        }

        /* trace of nxt = A^(cur_len+1) */
        seq_t tr = 0;
        for (idx_t i = 0; i < 28; ++i) {
            tr += nxt[i][i];
        }

        /* Append trace to cache */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (cache_len + 1));
        if (!tmp) {
            /* Allocation failure – abort gracefully */
            abort();
        }
        cache = tmp;
        cache[cache_len] = tr;
        cache_len++;
        cur_len++;

        /* Swap cur and nxt for next iteration */
        for (idx_t i = 0; i < 28; ++i)
            for (idx_t j = 0; j < 28; ++j)
                cur[i][j] = nxt[i][j];
    }

    return cache[n];
}
