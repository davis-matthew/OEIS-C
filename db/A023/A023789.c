/*
 * A023789  Katadromes: digits in base 7 are in strict descending order.
 * https://oeis.org/A023789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 104, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate all numbers whose base‑7 digits are strictly decreasing,
   store them in ascending order, and return the (n‑1)th element (1‑based) */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static bool cache_ready = false;

static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    if (!cache_ready) {
        /* There are 2^7 - 1 = 127 non‑empty subsets of {0,…,6} */
        cache_size = (1ULL << 7) - 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;      /* allocation failure – unlikely for tiny size */

        size_t pos = 0;
        for (uint64_t mask = 1; mask < (1ULL << 7); ++mask) {
            seq_t val = 0;
            for (int d = 6; d >= 0; --d) {
                if (mask & (1ULL << d)) {
                    val = val * (seq_t)7 + (seq_t)d; /* strict descending order */
                }
            }
            cache[pos++] = val;
        }

        qsort(cache, cache_size, sizeof(seq_t), cmp_seq);
        cache_ready = true;
    }

    size_t idx = (size_t)(n - 1);
    if (idx >= cache_size) return 0;   /* n beyond defined terms */
    return cache[idx];
}
