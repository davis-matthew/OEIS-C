/*
 * A147706  Number of partitions of n into parts having distinct digital roots
 *          (A010888).
 * https://oeis.org/A147706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of partitions of n into parts having distinct
 *            digital roots. Computation: dynamic programming over the nine
 *            possible digital roots. For each root d = 1..9 the algorithm
 *            loops over all parts v congruent to d (mod 9) and adds them to
 *            the partitions built from smaller roots. Complexity...
 * Range:     u8 n <= 32, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h> /* for memcpy, if desired */

static seq_t *memo = NULL;       /* memoized results */
static idx_t  memo_size = 0;     /* allocated size of memo */
static idx_t  computed_max = 0;  /* maximum n computed so far */

/* Compute a(0..N) values and store them in memo */
static void compute_up_to(idx_t N)
{
    /* Allocate or enlarge memo array */
    if (memo_size < N + 1) {
        free(memo);
        memo = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
        memo_size = N + 1;
    }

    /* Working arrays: dp (current), dp_prev (state before this root). */
    seq_t *dp      = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *dp_prev = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    dp[0] = (seq_t)1;  /* empty partition */

    /* Process each digital root d = 1..9 */
    for (int d = 1; d <= 9; ++d) {
        /* Copy current dp into dp_prev to preserve state before this root */
        for (idx_t i = 0; i <= N; ++i)
            dp_prev[i] = dp[i];

        /* Add one part for this root: values v ≡ d (mod 9) */
        for (idx_t v = d; v <= N; v += 9) {          /* possible part value */
            for (idx_t s = v; s <= N; ++s) {        /* sum value */
                dp[s] += dp_prev[s - v];
            }
        }
    }

    /* Store results into memo */
    for (idx_t i = 0; i <= N; ++i)
        memo[i] = dp[i];

    /* Free temporary arrays */
    free(dp);
    free(dp_prev);

    computed_max = N;
}

/* Public function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n > computed_max)
        compute_up_to(n);
    return memo[n];
}
