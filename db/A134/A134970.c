/*
 * A134970  Canyon numbers. Numbers with exactly one locally minimal digit and
 *          with exactly two locally maximal digits which are the same digit
 *          and nonadjacent.
 * https://oeis.org/A134970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 4681, u64 n <= 8557, u128 n <= 8557,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * This function generates all canyon numbers (OEIS A134970) and
 * returns the n‑th value (1‑based).  It builds a static cache of the
 * sorted sequence on the first call and then indexes it.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* comparison for qsort (unsigned seq_t values) */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

/* Build the cache of all canyon numbers in ascending order. */
static void build_cache(seq_t **out_ptr, idx_t *size_ptr)
{
    /* pre‑compute powers of 4 up to 4^8 */
    unsigned long long pow4[9];
    pow4[0] = 1ULL;
    for (int e = 1; e <= 8; ++e) pow4[e] = pow4[e-1] * 4ULL;

    unsigned long long total = 0;
    for (int d = 1; d <= 9; ++d) {
        for (int m = 0; m <= d-1; ++m) {
            int k = d - m - 1;          /* number of intermediate digits */
            total += pow4[k];
        }
    }

    seq_t *arr = (seq_t *)malloc((size_t)total * sizeof(seq_t));
    if (!arr) abort();
    unsigned long long idx = 0;

    for (int d = 1; d <= 9; ++d) {
        for (int m = 0; m <= d-1; ++m) {
            int k = d - m - 1;                   /* intermediate digits count */
            unsigned int maxmask = 1u << k;       /* 2^k possibilities */

            for (unsigned int maskL = 0; maskL < maxmask; ++maskL) {
                for (unsigned int maskR = 0; maskR < maxmask; ++maskR) {
                    seq_t val = 0;

                    /* left side: start with d */
                    val = val * 10 + d;
                    /* decreasing digits -----------------------------------*/
                    for (int i = k-1; i >= 0; --i) {
                        if (maskL & (1u << i)) {
                            val = val * 10 + (m + 1 + i);
                        }
                    }

                    /* middle minimum digit ------------------------------*/
                    val = val * 10 + m;

                    /* right side: increasing digits ----------------------*/
                    for (int i = 0; i < k; ++i) {
                        if (maskR & (1u << i)) {
                            val = val * 10 + (m + 1 + i);
                        }
                    }

                    /* final maximum digit ------------------------------*/
                    val = val * 10 + d;

                    arr[idx++] = val;
                }
            }
        }
    }

    /* sort the list */
    qsort(arr, (size_t)total, sizeof(seq_t), cmp_seq);

    *out_ptr  = arr;
    *size_ptr = (idx_t)total;
}

/* static cache variables */
static seq_t *canyon_cache  = NULL;
static idx_t  canyon_size   = 0;

/* the required function */
seq_t a(idx_t n)
{
    if (!canyon_cache) {
        build_cache(&canyon_cache, &canyon_size);
    }
    if (n == 0 || n > canyon_size) {
        return 0;
    }
    return canyon_cache[n-1];            /* a(1) is the first element */
}
