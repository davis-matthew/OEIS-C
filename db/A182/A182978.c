/*
 * A182978  Total number of parts that are the smallest part or the largest
 *          part in all partitions of n.
 * https://oeis.org/A182978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of parts that are smallest or largest * all
 *            partitions of n. * * The algorithm uses dynamic programming to
 *            accumulate the number of times * each part value appears as the
 *            smallest or the largest part. * Contributions are computed in
 *            O(n^2) time and memory }, * suitable for n u...
 * Range:     u8 n <= 11, u16 n <= 33, u32 n <= 107, u64 n <= 371,
 *            u128 n <= 1000, big any n
 * Helpers:   add_contributions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/add_contributions.h"

/* ------------------------------------------------------------ */
/*                static caching infrastructure                */
/* ------------------------------------------------------------ */
static seq_t *cachedA = NULL;   /* sequence a(n) */
static idx_t cachedSize = 0;    /* size of cachedA (max computed n) */

/* ------------------------------------------------------------ */
/*           auxiliary routines to update contributions        */
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*                        main routine                          */
/* ------------------------------------------------------------ */

/* recompute a(n) for all 0 ≤ n ≤ N */
static void recompute_up_to(idx_t N)
{
    /* allocate temporary arrays */
    seq_t *dp_le     = (seq_t*)calloc(N + 1, sizeof(seq_t));   /* partitions ≤ current part */
    seq_t *dp_ge     = (seq_t*)calloc(N + 1, sizeof(seq_t));   /* partitions ≥ current part (reverse) */
    seq_t *sum_large = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *sum_small = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *sum_all_equal = (seq_t*)calloc(N + 1, sizeof(seq_t));

    /* ------------- Partitions with parts ≤ current (forward DP) ------------- */
    dp_le[0] = 1;   /* empty partition */
    for (idx_t v = 1; v <= N; ++v) {
        /* contributions to largest part = v */
        add_contributions(N, v, dp_le, sum_large);

        /* update dp_le to allow part size v   */
        for (idx_t j = v; j <= N; ++j) {
            dp_le[j] += dp_le[j - v];
        }
    }

    /* ------------- Partitions with parts ≥ current (reverse DP) -------------- */
    dp_ge[0] = 1;   /* only empty partition fits parts > N */
    for (idx_t v = N; v >= 1; --v) {
        /* contributions to smallest part = v */
        add_contributions(N, v, dp_ge, sum_small);

        /* update dp_ge to allow part size v   */
        for (idx_t j = v; j <= N; ++j) {
            dp_ge[j] += dp_ge[j - v];
        }
    }

    /* ------------- Sum of parts that are all equal (subtract once) ------------- */
    for (idx_t n = 1; n <= N; ++n) {
        for (idx_t d = 1; d * d <= n; ++d) {
            if (n % d == 0) {
                idx_t other = n / d;
                sum_all_equal[n] += (seq_t)(n / d);   /* n / d  */
                if (other != d) {
                    sum_all_equal[n] += (seq_t)(n / other);    /* n / other */
                }
            }
        }
    }

    /* ------------- Assemble final sequence a(n) ------------- */
    seq_t *newA = (seq_t*)realloc(cachedA, (N + 1) * sizeof(seq_t));
    if (!newA) { /* allocation failure – abort gracefully */
        exit(EXIT_FAILURE);
    }
    cachedA = newA;
    cachedSize = N;
    cachedA[0] = 0;    /* unused (offset starts at 1) */
    for (idx_t n = 1; n <= N; ++n) {
        cachedA[n] = sum_small[n] + sum_large[n] - sum_all_equal[n];
    }

    /* free temporary arrays */
    free(dp_le);
    free(dp_ge);
    free(sum_large);
    free(sum_small);
    free(sum_all_equal);
}

/* ------------------------------------------------------------ */
/*                    external function a(idx_t n)                */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)0;     /* sequence is undefined for n=0, return 0 */
    }
    if (cachedSize < n) {
        recompute_up_to(n);
    }
    return cachedA[n];
}
