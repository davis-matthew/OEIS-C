/*
 * A162694  Strictly positive numbers n such that 36*n/(36+n) are integers.
 * https://oeis.org/A162694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) are the positive integers n for which 36*n/(36+n) is an
 *            integer. The value can be expressed as n = 36*k/(36-k) for k =
 *            1..35; the sequence is finite. The function computes the sorted
 *            list of such n values once and then returns the nth element
 *            (1‑indexed)
 * Range:     u8 none, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* sorted values  */
static size_t cache_len = 0;    /* number of values in cache */

static void build_cache(void)
{
    const idx_t K_MAX = 35;
    size_t count = 0;
    seq_t *tmp = (seq_t *)malloc(K_MAX * sizeof(seq_t));
    if (!tmp) return;  /* allocation failure – return empty cache */

    for (idx_t k = 1; k < 36; ++k) {
        seq_t numer = (seq_t)36 * k;
        seq_t denom = (seq_t)(36 - k);
        if (denom == 0) continue;
        if (numer % denom == 0) {
            tmp[count++] = numer / denom;
        }
    }

    /* Simple insertion sort (count ≤ 35). */
    for (size_t i = 1; i < count; ++i) {
        seq_t key = tmp[i];
        size_t j = i;
        while (j > 0 && tmp[j - 1] > key) {
            tmp[j] = tmp[j - 1];
            --j;
        }
        tmp[j] = key;
    }

    cache = tmp;
    cache_len = count;
}

seq_t a(idx_t n)
{
    if (cache_len == 0) build_cache();
    if (n == 0 || n > cache_len) return 0;
    return cache[n - 1];
}
