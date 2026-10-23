/*
 * A043659  Numbers whose base-13 representation has exactly 4 runs.
 * https://oeis.org/A043659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043659: Numbers whose base‑13 representation has exactly 4 runs. */
/* The function returns the n‑th smallest such integer (1‑based). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declaration of helper comparator */
static int cmp_seq(const void *, const void *);

/* Global cache of already generated terms */
static seq_t *cache = NULL;         /* dynamic array of terms      */
static size_t cache_len = 0;        /* number of terms stored     */
static size_t cache_cap = 0;        /* allocated capacity         */
static idx_t current_len = 4;       /* next length to generate    */

/* Ensure that the cache can hold at least 'need' elements.            */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4096;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;  /* allocation failure – will cause later crash,
                         * but such failure is unlikely in practice.    */
    cache = tmp;
    cache_cap = new_cap;
}

/* Comparator used by qsort for seq_t values.                            */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* Generate all numbers of exact length 'L' that have 4 runs in base 13
   and append them (in sorted order) to the global cache.                */
static void generate_length(idx_t L)
{
    /* There are C(L-1,3) compositions of L into 4 positive parts.     */
    for (idx_t l1 = 1; l1 <= L - 3; ++l1)
        for (idx_t l2 = 1; l2 <= L - l1 - 2; ++l2)
            for (idx_t l3 = 1; l3 <= L - l1 - l2 - 1; ++l3)
            {
                idx_t l4 = L - l1 - l2 - l3;          /* last run length */

                /* Number of digit combinations for this composition:
                   first run: 1..12, others: 0..12 but not equal to previous run byte. */
                const size_t batch_size = 12U * 12U * 12U * 12U;  /* 20736 */

                /* Allocate temporary storage for this batch.          */
                seq_t *temp = (seq_t *)malloc(batch_size * sizeof(seq_t));
                if (!temp) return; /* allocation failure – ignore.               */

                size_t idx = 0;
                for (int d1 = 1; d1 <= 12; ++d1)
                    for (int d2 = 0; d2 <= 12; ++d2) if (d2 != d1)
                        for (int d3 = 0; d3 <= 12; ++d3) if (d3 != d2)
                            for (int d4 = 0; d4 <= 12; ++d4) if (d4 != d3)
                            {
                                seq_t val = 0;
                                for (idx_t i = 0; i < l1; ++i) val = val * 13 + (seq_t)d1;
                                for (idx_t i = 0; i < l2; ++i) val = val * 13 + (seq_t)d2;
                                for (idx_t i = 0; i < l3; ++i) val = val * 13 + (seq_t)d3;
                                for (idx_t i = 0; i < l4; ++i) val = val * 13 + (seq_t)d4;
                                temp[idx++] = val;
                            }

                /* Sort the freshly generated numbers.                 */
                qsort(temp, batch_size, sizeof(seq_t), cmp_seq);

                /* Append to global cache.                           */
                ensure_cache_capacity(cache_len + batch_size);
                for (size_t i = 0; i < batch_size; ++i)
                    cache[cache_len++] = temp[i];

                free(temp);
            }
}

/* Main requested function.                                            */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1 – return 0 as a safe default */

    /* Generate terms until we have at least n in the cache.           */
    while (cache_len < n)
        generate_length(current_len++);
    return cache[n - 1];
}
