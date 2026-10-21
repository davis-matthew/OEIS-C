/*
 * A221620  Number of n X 5 arrays with each row a permutation of 1..5 having
 *          at least as many downsteps as the preceding row.
 * https://oeis.org/A221620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A221620 – number of n×5 arrays of permutations of 1..5
   where each row has at least as many downsteps (adjacent decreases)
   as the preceding row.
   Computed by dynamic programming on the 5 possible downstep counts
   (0..4).  For each row the number of permutations with exactly d
   downsteps is the Eulerian number A(5,d): 1,26,66,26,1.
   The recurrence for the DP sums is:
        f_n[d] = c[d] * Σ_{i=0..d} f_{n-1}[i]
   and a(n) = Σ_{d=0..4} f_n[d].
   Initial values: a(1)=120, a(2)=10055, a(3)=738150, a(4)=51149685,
   a(5)=3451956516.
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

/* Memoised cache for a(n).  Index 0 is unused. */
static seq_t *cache_a = NULL;
static size_t cache_cap = 0;
static idx_t cache_last_n = 0;

/* Current DP values for the last computed row. */
static seq_t f_cur[5] = {0};

/* Whether the DP has been initialised. */
static bool dp_initialised = false;

/* Eulerian numbers for n=5: number of permutations with d downsteps. */
static const seq_t c[5] = {1, 26, 66, 26, 1};

/* Ensure cache_a can hold values up to n (including index n). */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < needed) new_cap <<= 1;
        cache_a = seq_realloc(cache_a, cache_cap, new_cap);
        cache_cap = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* a(0) is unused, 0 is a safe return */

    /* Initialise DP on first call. */
    if (!dp_initialised) {
        for (int d = 0; d < 5; ++d) f_cur[d] = c[d];
        seq_t sum1 = 0;
        for (int d = 0; d < 5; ++d) sum1 += f_cur[d];
        ensure_cache_size(1);
        cache_a[1] = sum1;
        cache_last_n = 1;
        dp_initialised = true;
    }

    /* Return cached value if already computed. */
    if (n <= cache_last_n) return cache_a[n];

    /* Compute rows up to n, extending cache as needed. */
    ensure_cache_size(n);
    for (idx_t k = cache_last_n + 1; k <= n; ++k) {
        seq_t new_f[5];
        seq_t cum = 0;
        seq_t sum_k = 0;
        for (int d = 0; d < 5; ++d) {
            cum += f_cur[d];
            new_f[d] = c[d] * cum;
            sum_k += new_f[d];
        }
        for (int d = 0; d < 5; ++d) f_cur[d] = new_f[d];
        cache_a[k] = sum_k;
    }

    cache_last_n = n;
    return cache_a[n];
}
