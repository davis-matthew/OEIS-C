/*
 * A182984  Total number of parts that are not the smallest part in all
 *          partitions of n.
 * https://oeis.org/A182984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sum of the number of parts over all partitions of n) * -
 *            (sum of the multiplicity of the smallest part over all
 *            partitions of n)
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 104, u64 n <= 359,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* Cached values for 0 .. cache_size-1 */
static seq_t *sum_parts_cache = NULL;
static seq_t *g_part_cache   = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    size_t N = (size_t)n;

    /* Recompute tables only when a larger n is requested */
    if (N >= cache_size) {
        size_t new_size = N + 1;

        /* Allocate fresh tables */
        seq_t *sum_arr = (seq_t*)calloc(new_size, sizeof(seq_t));
        seq_t *g_arr   = (seq_t*)calloc(new_size, sizeof(seq_t));

        /* ---- 1. Sum of parts over all partitions ---- */
        seq_t *p = (seq_t*)calloc(new_size, sizeof(seq_t));        /* partition counts */
        seq_t *sum = (seq_t*)sum_arr;                              /* same buffer as sum_arr */
        p[0]       = 1;
        sum[0]     = 0;

        for (size_t part = 1; part <= N; ++part) {
            for (size_t j = part; j <= N; ++j) {
                p[j]    += p[j - part];
                sum[j]  += sum[j - part] + p[j - part];
            }
        }

        /* ---- 2. Sum of multiplicity of the smallest part ---- */
        /* B[t] = number of partitions of t with parts >= current s
           prev[t] holds the values before the current s step            */
        seq_t *B    = (seq_t*)calloc(new_size, sizeof(seq_t));
        seq_t *prev = (seq_t*)malloc(new_size * sizeof(seq_t));
        B[0] = 1;                    /* partitions of 0 with any parts = 1 */

        for (size_t s = N; s >= 1; --s) {
            /* copy current B into prev before it is updated */
            memcpy(prev, B, new_size * sizeof(seq_t));

            /* accumulate contributions of the smallest part = s */
            for (size_t n = s; n <= N; ++n) {
                size_t t = n - s;
                size_t m = 1;
                while (1) {
                    /* add m * prev[t] */
                    g_arr[n] += (seq_t)m * prev[t];
                    if (t < s) break;   /* next remainder would be negative */
                    t -= s;
                    ++m;
                }
            }

            /* update B to include part size s (partitions with parts >= s) */
            for (size_t t = s; t <= N; ++t) {
                B[t] += B[t - s];
            }
        }

        /* Free temporary arrays */
        free(p);
        free(prev);
        free(B);

        /* Replace old cached tables */
        if (sum_parts_cache) free(sum_parts_cache);
        if (g_part_cache)   free(g_part_cache);
        sum_parts_cache = sum_arr;
        g_part_cache   = g_arr;
        cache_size     = new_size;
    }

    /* Final result */
    return sum_parts_cache[n] - g_part_cache[n];
}
