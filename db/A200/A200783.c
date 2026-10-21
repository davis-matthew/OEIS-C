/*
 * A200783  G.f.: 1/(1-7*x+35*x^3-35*x^4+7*x^6-x^7).
 * https://oeis.org/A200783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of words of length n over the alphabet {0,…,6} in
 *            which no strictly decreasing consecutive factor of length 3
 *            occurs
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   copy_matrix, zero_matrix
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  We compute the sequence by dynamic programming: 
    DA[i][a][b] = number of words of length i that end with the two
    letters a,b (0 ≤ a,b ≤ 6).  From DA[i] we obtain DA[i+1] by appending
    a letter c to all valid words.  The property is violated precisely when
    a>b>c.  Hence a transition from (a,b) to (b,c) is allowed iff not
    (a>b && b>c).  This DP works for all n ≥ 0 and can be reused for
    arbitrary call order by memoising the computed values. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/copy_matrix.h"
#include "../utils/zero_matrix.h"

/* The caller supplies seq_t and idx_t.  We use seq_t for arithmetic. */
static seq_t *cache = NULL;          /* memoised a(n) values */
static idx_t  cache_len = 0;        /* number of values stored (cache_len = last index + 1) */

static seq_t cur[7][7];   /* DP array for current length */
static seq_t nxt[7][7];   /* DP array for next length */

seq_t a(idx_t n) {
    /* Handle the trivial cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)49;

    /* Initialise the cache and DP state once */
    if (cache_len == 0) {
        size_t init_cap = 16;
        if (init_cap < 3) init_cap = 3;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failed – behaviour undefined */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)49;
        cache_len = 3;                     /* cached indices 0,1,2 */

        /* DP state for length 2: every pair (a,b) occurs exactly once */
        for (int a_i = 0; a_i < 7; ++a_i)
            for (int b_i = 0; b_i < 7; ++b_i)
                cur[a_i][b_i] = (seq_t)1;
    }

    /* If the value is already cached, return it immediately */
    if (n < cache_len) return cache[n];

    /* Grow the cache if necessary */
    if (n + 1 > (size_t)cache_len) {
        size_t new_cap = cache_len * 2;
        if (new_cap == 0) new_cap = 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failed */
        cache = new_cache;
    }

    /* Extend the DP until it reaches the requested index n */
    while (cache_len <= n) {
        /* Compute DP for length (cache_len) from current state cur (length cache_len-1) */
        zero_matrix(nxt);
        for (int a = 0; a < 7; ++a) {
            for (int b = 0; b < 7; ++b) {
                seq_t val = cur[a][b];
                for (int c = 0; c < 7; ++c) {
                    if (!(a > b && b > c)) {
                        nxt[b][c] += val;
                    }
                }
            }
        }

        /* Sum all entries of nxt to obtain a(cache_len) */
        seq_t sum = 0;
        for (int b = 0; b < 7; ++b)
            for (int c = 0; c < 7; ++c)
                sum += nxt[b][c];

        cache[cache_len] = sum;

        /* Prepare for the next iteration */
        copy_matrix(cur, nxt);
        ++cache_len;
    }

    return cache[n];
}
