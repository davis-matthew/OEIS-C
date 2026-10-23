/*
 * A025161  Number of partitions of n with distinct parts p(i) such that if i
 *          != j, then |p(i) - p(j)| >= 7.
 * https://oeis.org/A025161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = ∑_{k≥1, 7·k·(k-1)/2 ≤ n} p_k(n – 7·k·(k-1)/2) * where
 *            p_k(m) is the number of partitions of m into exactly k positive
 *            parts * (equivalently, the number of partitions of m‑k into at
 *            most k parts, with * parts possibly equal). This counts
 *            partitions of n with distinct parts * differing by...
 * Range:     u8 n <= 63, u16 n <= 167, u32 n <= 501, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static struct {
    seq_t *data;   /* flattened array: data[k*cols + s] = partitions of s into at most k parts */
    idx_t n;       /* largest n for which data is valid */
    idx_t k;       /* largest number of parts considered for current n */
} cache = {NULL, 0, 0};

/* Allocate an array of seq_t, zero‑initialised.  Works for both C and C++. */
static inline seq_t *make_array(idx_t size)
{
#ifdef __cplusplus
    return static_cast<seq_t*>(operator new[](size * sizeof(seq_t), std::nothrow));
#else
    return (seq_t*)calloc((size_t)size, sizeof(seq_t));
#endif
}

/* Free an array allocated by make_array. */
static inline void free_array(seq_t *p)
{
#ifdef __cplusplus
    operator delete[](p);
#else
    free(p);
#endif
}

/* Rebuild the partition table for all n ≤ N and all k with
 * 7·k·(k-1)/2 ≤ N. */
static void rebuild_cache(idx_t N)
{
    /* compute the largest k needed for this N */
    idx_t kmax = 1;
    while (7 * kmax * (kmax - 1) / 2 <= N) kmax++;
    kmax--;                     /* last value that satisfies the inequality */
    idx_t cols = N + 1;         /* number of columns in the 2‑D array */

    /* allocate new data */
    seq_t *new_data = make_array((kmax + 1) * cols);
    if (!new_data) return;     /* allocation failed – caller will get 0 */

    /* base case: one way to partition 0 into 0 parts */
    new_data[0] = (seq_t)1;

    /* dynamic programming: number of partitions of s into at most k parts */
    for (idx_t k = 1; k <= kmax; ++k) {
        for (idx_t s = 0; s < cols; ++s) {
            seq_t val = 0;
            if (s >= k)          /* use a part of size k */
                val += new_data[k * cols + s - k];
            val += new_data[(k - 1) * cols + s]; /* do not use part k */
            new_data[k * cols + s] = val;
        }
    }

    /* free old data and store the new one */
    if (cache.data) free_array(cache.data);
    cache.data = new_data;
    cache.n = N;
    cache.k = kmax;
}

/* Public OEIS function.  Not marked static to give it external linkage. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset is 1; n=0 is outside the definition */

    /* rebuild the table if we haven't covered this n yet */
    if (n > cache.n || cache.k == 0) rebuild_cache(n);

    seq_t result = 0;
    idx_t cols = cache.n + 1;

    for (idx_t k = 1; k <= cache.k; ++k) {
        idx_t penalty = 7 * k * (k - 1) / 2;
        if (penalty > n) break;
        idx_t m = n - penalty;               /* remaining sum after removing the k·(k-1)/2*7 term */
        if (m < k) continue;                 /* cannot partition m into k positive parts */
        /* partitions of m into exactly k parts = partitions of (m‑k) into at most k parts */
        seq_t add = cache.data[k * cols + (m - k)];
        result += add;
    }
    return result;
}
