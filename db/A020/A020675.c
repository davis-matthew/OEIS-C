/*
 * A020675  Numbers of form 2 x^2 + 7 y^2.
 * https://oeis.org/A020675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A020675: Numbers of the form 2*x^2 + 7*y^2.  The function
 * builds the sequence on demand by generating all representable
 * numbers up to a bound, deduplicating, sorting, and caching them.
 * Subsequent calls reuse the cache. */

#include <stdlib.h>
#include <math.h>

/* comparator for qsort – not one of the forbidden names */
static int seq_cmp(const void *a, const void *b)
{
    seq_t av = *(const seq_t *)a;
    seq_t bv = *(const seq_t *)b;
    if (av < bv) return -1;
    if (av > bv) return 1;
    return 0;
}

/* static cache: sorted distinct values <= current_limit */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t current_limit = 0;

/* ensure that the cache contains at least n distinct values   */
/* by generating all numbers <= current_limit, increasing the limit if needed */
static void ensure_cache(idx_t n)
{
    if (cache_size >= (size_t)n)
        return;                                   /* already enough */

    if (current_limit == 0)
        current_limit = n * 2;                      /* initial guess */

    while (1) {
        /* bounds for x and y so that 2*x^2 <= current_limit, 7*y^2 <= current_limit */
        idx_t Xmax = (idx_t)sqrt((double)current_limit / 2.0) + 1;
        idx_t Ymax = (idx_t)sqrt((double)current_limit / 7.0) + 1;

        /* maximum number of pairs to try */
        size_t total_pairs = (size_t)(Xmax + 1) * (Ymax + 1);
        seq_t *temp = (seq_t *)malloc(total_pairs * sizeof(seq_t));
        if (!temp) exit(1);

        size_t count = 0;
        /* generate all values <= current_limit */
        for (idx_t y = 0; y <= Ymax; ++y) {
            idx_t ysq = y * y;
            idx_t part_y = 7 * ysq;
            for (idx_t x = 0; x <= Xmax; ++x) {
                idx_t xsq = x * x;
                idx_t val64 = part_y + 2 * xsq;
                if (val64 <= current_limit) {
                    temp[count++] = (seq_t)val64;
                }
            }
        }

        /* sort and deduplicate */
        qsort(temp, count, sizeof(seq_t), seq_cmp);

        size_t unique = 0;
        if (count > 0) unique = 1;
        for (size_t i = 1; i < count; ++i) {
            if (temp[i] != temp[i - 1]) unique++;
        }

        seq_t *new_cache = NULL;
        if (unique > 0) {
            new_cache = (seq_t *)malloc(unique * sizeof(seq_t));
            if (!new_cache) {
                free(temp);
                exit(1);
            }
            new_cache[0] = temp[0];
            size_t j = 1;
            for (size_t i = 1; i < count; ++i) {
                if (temp[i] != temp[i - 1]) {
                    new_cache[j++] = temp[i];
                }
            }
        }

        free(temp);
        free(cache);
        cache = new_cache;
        cache_size = unique;

        if (cache_size >= (size_t)n)
            break;                                  /* enough values are present */

        /* need more numbers – increase limit and retry */
        if (current_limit < (idx_t)UINT64_MAX / 2)
            current_limit *= 2;
        else
            break;                                  /* safety: cannot grow further */
    }
}

/* Sequence function with 1‑based indexing */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                           /* undefined, return 0 */
    ensure_cache(n);
    return cache[n - 1];
}
