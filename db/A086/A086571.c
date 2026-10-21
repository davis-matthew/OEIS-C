/*
 * A086571  a(1)=1, for n>1: a(n) = smallest divisor of n occurring earlier at
 *          most once.
 * https://oeis.org/A086571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest divisor of n that has appeared at most once
 *            among a(1)…a(n‑1)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* comparison for qsort on idx_t */
static int cmp_idx(const void *a, const void *b) {
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    return (va < vb) ? -1 : (va > vb);
}

/* sequence a(n) defined in OEIS A086571 */
seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* frequency of term values seen so far (only indices up to current n are needed) */
    static uint32_t *freq = NULL;
    static size_t freq_size = 0;

    /* highest n for which a(k) has been computed */
    static idx_t max_computed = 0;

    if (n == 0)                /* not defined in OEIS, but avoid UB */
        return (seq_t)0;

    /* expand cache if needed */
    if ((size_t)n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!tmp) abort();    /* simple OOM handling */
        cache = tmp;
        cache_size = (size_t)n + 1;
    }

    /* expand frequency array if needed */
    if ((size_t)n + 1 > freq_size) {
        uint32_t *tmp = (uint32_t *)realloc(freq, ((size_t)n + 1) * sizeof(uint32_t));
        if (!tmp) abort();
        freq = tmp;
        for (size_t i = freq_size; i < (size_t)n + 1; ++i)
            freq[i] = 0;
        freq_size = (size_t)n + 1;
    }

    /* compute terms up to n if necessary */
    for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
        seq_t val;

        if (cur == 1) {
            val = (seq_t)1;
        } else {
            /* gather all divisors of cur */
            size_t div_cnt = 0, div_cap = 16;
            idx_t *divs = (idx_t *)malloc(div_cap * sizeof(idx_t));
            for (idx_t d = 1; d * d <= cur; ++d) {
                if (cur % d == 0) {
                    if (div_cnt == div_cap) {
                        div_cap *= 2;
                        divs = (idx_t *)realloc(divs, div_cap * sizeof(idx_t));
                    }
                    divs[div_cnt++] = d;
                    if (d * d != cur) {
                        if (div_cnt == div_cap) {
                            div_cap *= 2;
                            divs = (idx_t *)realloc(divs, div_cap * sizeof(idx_t));
                        }
                        divs[div_cnt++] = cur / d;
                    }
                }
            }
            qsort(divs, div_cnt, sizeof(idx_t), cmp_idx);

            /* choose the smallest divisor that has occurred at most once so far */
            seq_t candidate = (seq_t)cur;              /* fallback, should never be needed */
            for (size_t i = 0; i < div_cnt; ++i) {
                idx_t d = divs[i];
                if (freq[d] <= 1) {
                    candidate = (seq_t)d;
                    break;
                }
            }
            free(divs);
            val = candidate;
        }

        cache[cur] = val;
        /* update frequency table (values are <= cur, so within freq array) */
        if ((size_t)val < freq_size)
            freq[(size_t)val] += 1;
    }

    max_computed = n;
    return cache[n];
}
