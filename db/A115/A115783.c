/*
 * A115783  Numbers that can be represented in the form (m+1)^k - m^k in at
 *          least two ways.
 * https://oeis.org/A115783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers that can be represented in the form (m+1)^k - m^k in at
 *            least two distinct ways (m≥0, k≥0). Offset: 1. The sequence
 *            starts 0, 1, 7, 15, 19, … as given in OEIS A115783
 * Range:     u8 n <= 1, u16 n <= 41, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Basic includes and utilities */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* Compare function for qsort on seq_t values */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Static cache for lazy one‑time generation */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static bool initialized = false;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* ----------------------------------------------------------- */
        /* 1. Generate all (m+1)^k - m^k with k≥3 and value ≤ MAXVAL   */
        /* ----------------------------------------------------------- */
        const seq_t MAXVAL = (seq_t)2000000000ULL;   /* safety bound   */
        size_t cap = 8192;
        seq_t *vals = (seq_t *)malloc(cap * sizeof(seq_t));
        size_t sz = 0;
        if (!vals) return (seq_t)0;

        /* For k=3 … 20 (higher ks produce only 1 for m=0, already counted) */
        for (idx_t k = 3; k <= 20; ++k) {
            /* For each m until value > MAXVAL */
            for (idx_t m = 0; /* loop inside */; ++m) {
                /* (m+1)^k - m^k */
                seq_t a_pow = big_pow((seq_t)(m + 1), (seq_t)k);
                seq_t b_pow = big_pow((seq_t)m, (seq_t)k);
                seq_t val = a_pow - b_pow;
                if (val > MAXVAL) break;
                if (sz == cap) {                     /* grow array      */
                    cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(vals, cap * sizeof(seq_t));
                    if (!tmp) { free(vals); return (seq_t)0; }
                    vals = tmp;
                }
                vals[sz++] = val;
            }
        }

        /* ----------------------------------------------------------- */
        /* 2. Sort and compress duplicates (count occurrences)        */
        /* ----------------------------------------------------------- */
        qsort(vals, sz, sizeof(seq_t), cmp_seq);

        size_t uniq_cap = sz;
        seq_t *uniq_vals = (seq_t *)malloc(uniq_cap * sizeof(seq_t));
        size_t *uniq_cnt = (size_t *)malloc(uniq_cap * sizeof(size_t));
        if (!uniq_vals || !uniq_cnt) { free(vals); return (seq_t)0; }

        size_t i = 0, u = 0;
        while (i < sz) {
            size_t j = i + 1;
            while (j < sz && vals[j] == vals[i]) ++j;
            uniq_vals[u] = vals[i];
            uniq_cnt[u] = j - i;              /* number of occurrences with k≥3 */
            ++u;
            i = j;
        }
        free(vals);

        /* ----------------------------------------------------------- */
        /* 3. Build final list: count representation sources, keep
             value if it has at least two overall representations.   */
        /* ----------------------------------------------------------- */
        size_t final_cap = u + 1;    /* +1 for the number 0 */
        seq_t *final_vals = (seq_t *)malloc(final_cap * sizeof(seq_t));
        if (!final_vals) { free(uniq_vals); free(uniq_cnt); return (seq_t)0; }
        size_t fsz = 0;

        /* 0 is always in the sequence (k=0, all m give 0) */
        final_vals[fsz++] = (seq_t)0;

        for (size_t idx = 0; idx < u; ++idx) {
            seq_t val = uniq_vals[idx];
            size_t total = uniq_cnt[idx];       /* from k≥3 */

            if (val == (seq_t)1) {
                /* k=1 gives 1 */
                total += 1;
            }
            if ((val & (seq_t)1) == (seq_t)1) {
                /* k=2 produces all odd numbers */
                total += 1;
            }
            /* 1 already counted, 0 never appears here */

            if (total >= 2) {
                if (fsz == final_cap) {          /* grow if needed */
                    final_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(final_vals, final_cap * sizeof(seq_t));
                    if (!tmp) { free(uniq_vals); free(uniq_cnt); free(final_vals); return (seq_t)0; }
                    final_vals = tmp;
                }
                final_vals[fsz++] = val;
            }
        }

        free(uniq_vals);
        free(uniq_cnt);

        /* ----------------------------------------------------------- */
        /* 4. Store in static cache and report length                  */
        /* ----------------------------------------------------------- */
        seq_cache = final_vals;
        seq_len = fsz;
        initialized = true;
    }

    if (n == 0 || n > seq_len) return (seq_t)0;
    return seq_cache[n - 1];
}
