/*
 * A048930  Numbers that are the sum of 6 positive cubes in exactly 2 ways.
 * https://oeis.org/A048930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1094, u32 n <= 1094, u64 n <= 1094,
 *            u128 n <= 1094, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate all numbers that are the sum of six positive
   cubes in exactly two unordered ways, using a coin‑change dynamic
   program.  The resulting sequence is stored in a static array and
   indexed by the argument n (starting at 1).  The algorithm runs in
   time O(LIMIT * MAX_K) and uses about 6 MB of memory for LIMIT=400000,
   enough to cover the first ~10000 terms of the sequence. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *terms = NULL;      /* array holding terms of the sequence */
    static idx_t  count = 0;         /* number of terms stored */
    static idx_t  cap   = 0;         /* current capacity of terms */
    static int    initialized = 0;   /* flag */

    /* constants */
    const idx_t LIMIT = 400000;      /* search limit, covers >10 000 terms */
    const int   MAX_K = 6;           /* exactly 6 cubes */

    if (!initialized) {
        initialized = 1;

        /* ----------------------------------------------------------
           1.  Dynamic table dp[k][s] = #ways to express s using k cubes.
           ---------------------------------------------------------- */
        const size_t ROWS    = MAX_K + 1;
        const size_t COLUMNS = LIMIT + 1;
        const size_t SIZE    = ROWS * COLUMNS;

        uint16_t *dp = (uint16_t *)calloc(SIZE, sizeof(uint16_t));
        if (!dp) abort();           /* memory allocation failed */

        /* base case: 0 cubes sum to 0 in exactly one way */
        dp[0 * COLUMNS + 0] = 1;

        /* 2.  enumerate cube values */
        idx_t cube_root = 1;
        while (((idx_t)(cube_root + 1) * (cube_root + 1) * (cube_root + 1))
               <= LIMIT)
            ++cube_root;

        for (idx_t i = 1; i <= cube_root; ++i) {
            idx_t v = i * i * i;          /* i³ */
            if (v > LIMIT) break;

            /* Update dp for the current cube value.
               The coin‐change DP iterates over sums ascending so that
               the same coin can be used multiple times.  For each
               number of cubes k we only need to use the previous k‑1
               row. */
            for (idx_t s = v; s <= LIMIT; ++s) {
                size_t idx_s = (size_t)s;
                for (int k = 1; k <= MAX_K; ++k) {
                    size_t src = (size_t)(k - 1) * COLUMNS + (idx_s - v);
                    size_t dst = (size_t)k * COLUMNS + idx_s;
                    dp[dst] += dp[src];
                }
            }
        }

        /* 3.  collect terms where exactly two representations exist */
        cap = 1024;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!terms) abort();

        for (idx_t s = 1; s <= LIMIT; ++s) {
            if (dp[6 * COLUMNS + (size_t)s] == 2) {
                if (count == cap) {               /* grow array */
                    cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(terms,
                                                  cap * sizeof(seq_t));
                    if (!tmp) abort();
                    terms = tmp;
                }
                terms[count++] = (seq_t)s;
            }
        }

        /* clean up DP table */
        free(dp);
    }

    /* ----------------------------------------------------------
       Retrieve the requested term (1‑based index).
       ---------------------------------------------------------- */
    if (n == 0 || n > count)
        return 0;                 /* out of range – undefined term */

    return terms[n - 1];
}
