/*
 * A039809  For n > 1, a(n) doubles under the transform T, where Ta is the
 *          matrix product of partition triangle A008284 with a, with a(1) =
 *          1.
 * https://oeis.org/A039809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(n) = Sum_{i=1..n-1} A008284(n,i) * a(i), a(1) = 1 where
 *            A008284(n,i) = number of partitions of n into exactly i parts.
 *            The implementation is O(n^2) per recomputation up to the
 *            requested n
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *a_cache = NULL;      /* a[0 .. size-1] */
static idx_t  cache_size = 0;      /* current allocated size */

/* recompute the sequence up to N (inclusive) */
static void recompute_upto(idx_t N)
{
    /* resize the a array */
    seq_t *tmp = (seq_t *)realloc(a_cache, (N + 1) * sizeof(seq_t));
    if (!tmp)
        abort();
    a_cache = tmp;

    /* initialise a[1] = 1; zero others */
    a_cache[0] = 0;
    if (N >= 1)
        a_cache[1] = 1;
    for (idx_t i = 2; i <= N; ++i)
        a_cache[i] = 0;

    /* temporary array for partitions of m into parts <= i */
    seq_t *f = (seq_t *)calloc((N + 1), sizeof(seq_t));
    if (!f)
        abort();
    f[0] = 1;   /* one partition of 0 */

    /* main double loop */
    for (idx_t i = 1; i <= N - 1; ++i) {
        /* update f for part size i: partitions of m with parts <= i */
        for (idx_t m = i; m <= N - 1; ++m)
            f[m] += f[m - i];

        /* add contributions for all targets > i */
        seq_t ai = a_cache[i];
        for (idx_t target = i + 1; target <= N; ++target) {
            /* T(target,i) = f[target-i] */
            seq_t contrib = ai * f[target - i];
            a_cache[target] += contrib;
        }
    }

    free(f);
}

seq_t a(idx_t n)
{
    /* ensure the cache covers n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        recompute_upto(n);
        cache_size = new_size;
    }
    return a_cache[n];
}
