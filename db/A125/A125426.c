/*
 * A125426  Number of base 14 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) * where A is the 14×14 adjacency matrix with *
 *            A[i][j] = 1 if |i-j| <= 7, else 0. * a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case for the empty string */
    if (n == 0) return 1;

    /* Static state: cache for trace values and current matrix power */
    static seq_t *trace_cache = NULL;   /* trace_cache[i] = a(i) */
    static idx_t cache_size = 0;        /* allocated size of trace_cache */
    static idx_t max_computed = 0;      /* largest n for which we have a(n) */
    static seq_t power[14][14];         /* current A^k, where k = max_computed */

    /* Static adjacency matrix */
    static bool adjacency_initialized = false;
    static seq_t adjacency[14][14];

    /* Initialize adjacency matrix once */
    if (!adjacency_initialized) {
        for (idx_t i = 0; i < 14; ++i) {
            for (idx_t j = 0; j < 14; ++j) {
                seq_t diff = (i > j) ? (i - j) : (j - i);
                adjacency[i][j] = (diff <= 7) ? 1 : 0;
            }
        }
        adjacency_initialized = true;
    }

    /* Allocate or grow the cache if needed */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        trace_cache = (seq_t *)realloc(trace_cache, new_size * sizeof(seq_t));
        /* The new element(s) may be uninitialized, but we set trace_cache[0] with
         * the base case and others will be filled when computed */
        cache_size = new_size;
    }

    /* Ensure the base case is stored */
    if (cache_size > 0 && trace_cache[0] != 1) /* first time or after reallocation */
        trace_cache[0] = 1;

    /* If nothing computed yet, compute A^1 and a(1) */
    if (max_computed == 0) {
        for (idx_t i = 0; i < 14; ++i)
            for (idx_t j = 0; j < 14; ++j)
                power[i][j] = adjacency[i][j];
        seq_t tr = 0;
        for (idx_t i = 0; i < 14; ++i) tr += power[i][i];
        trace_cache[1] = tr;
        max_computed = 1;
    }

    /* Compute additional powers up to requested n */
    if (n > max_computed) {
        seq_t next[14][14];
        for (idx_t k = max_computed + 1; k <= n; ++k) {
            /* Multiply current power by adjacency: next = power * adjacency */
            for (idx_t i = 0; i < 14; ++i) {
                for (idx_t j = 0; j < 14; ++j) {
                    seq_t sum = 0;
                    for (idx_t t = 0; t < 14; ++t)
                        sum += power[i][t] * adjacency[t][j];
                    next[i][j] = sum;
                }
            }
            /* Store trace of next matrix */
            seq_t tr = 0;
            for (idx_t i = 0; i < 14; ++i) tr += next[i][i];
            trace_cache[k] = tr;
            /* Update current power matrix for next iteration */
            for (idx_t i = 0; i < 14; ++i)
                for (idx_t j = 0; j < 14; ++j)
                    power[i][j] = next[i][j];
        }
        max_computed = n;
    }

    return trace_cache[n];
}
