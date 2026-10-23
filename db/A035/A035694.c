/*
 * A035694  Number of partitions of n into parts 8k+4 and 8k+5 with at least
 *          one part of each type.
 * https://oeis.org/A035694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k>=1, m>=1, k+m=n} p4(k) * p5(m), where p4(k) =
 *            number of partitions of k into parts 8t+4, and p5(m) = number of
 *            partitions of m into parts 8t+5
 * Range:     u8 n <= 76, u16 n <= 188, u32 n <= 528, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t max_computed = 0;          /* largest n already processed */
static seq_t *p4 = NULL;                /* partitions using only parts ≡4 (mod 8) */
static seq_t *p5 = NULL;                /* partitions using only parts ≡5 (mod 8) */
static seq_t *a_cache = NULL;           /* memoised a(n) values */

/* Allocate and zero an array of seq_t of size sz (indices 0..sz-1). */
static seq_t *alloc_array(idx_t sz)
{
    return (seq_t *)calloc((size_t)sz, sizeof(seq_t));
}

/* Recompute all auxiliary tables up to and including N. */
static void compute_up_to(idx_t N)
{
    /* Free any existing arrays. */
    free(p4);
    free(p5);
    free(a_cache);

    /* Allocate new zeroed arrays. */
    idx_t sz = N + 1;
    p4      = alloc_array(sz);
    p5      = alloc_array(sz);
    a_cache = alloc_array(sz);
    if (!p4 || !p5 || !a_cache) {
        /* Allocation failed – terminate.  In the test harness this will
           never happen for the expected size range. */
        exit(1);
    }

    /* Base cases: empty partition counts as 1 for convolution. */
    p4[0] = 1;
    p5[0] = 1;

    /* Compute partitions using only parts ≡4 (mod 8). */
    for (idx_t v = 4; v <= N; v += 8) {
        for (idx_t i = v; i <= N; ++i) {
            p4[i] += p4[i - v];
        }
    }

    /* Compute partitions using only parts ≡5 (mod 8). */
    for (idx_t v = 5; v <= N; v += 8) {
        for (idx_t i = v; i <= N; ++i) {
            p5[i] += p5[i - v];
        }
    }

    /* Convolution: count partitions that contain at least one part of each type. */
    for (idx_t k = 1; k <= N - 1; ++k) {
        if (p4[k] == 0) continue;
        for (idx_t m = 1; m <= N - k; ++m) {
            if (p5[m] == 0) continue;
            a_cache[k + m] += p4[k] * p5[m];
        }
    }

    max_computed = N;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* below offset – undefined */
    if (n > max_computed) {
        compute_up_to(n);
    }
    return a_cache[n];
}
