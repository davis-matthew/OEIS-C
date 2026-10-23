/*
 * A147842  Sum of all balanced numbers less than 10^n.
 * https://oeis.org/A147842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=1..n} A147808(k) (partial sums of balanced
 *            numbers)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


static size_t cache_a_cap   = 0;
static seq_t *cache_a       = NULL;
static size_t cache_max     = 0;

static size_t cur_m   = 0;
static size_t max_range = 0;

static seq_t *cntL = NULL;
static seq_t *sumL = NULL;
static seq_t *cntR = NULL;
static seq_t *sumR = NULL;

static seq_t *pow10 = NULL;
static size_t pow10_cap = 0;

/* Extend cache array as needed */
static void ensure_cache(size_t needed)
{
    if (needed < cache_a_cap)
        return;
    size_t new_cap = needed + 1;
    seq_t *tmp = (seq_t *)realloc(cache_a, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    for (size_t i = cache_a_cap; i < new_cap; ++i)
        tmp[i] = (seq_t)0;
    cache_a = tmp;
    cache_a_cap = new_cap;
}

/* Extend pow10 array to contain 10^e (e inclusive) */
static void ensure_pow10(size_t e)
{
    if (pow10_cap > e)
        return;
    size_t new_cap = e + 1;
    seq_t *tmp = (seq_t *)realloc(pow10, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    if (pow10_cap == 0) {
        tmp[0] = (seq_t)1;
        for (size_t i = 1; i < new_cap; ++i)
            tmp[i] = tmp[i - 1] * (seq_t)10;
    } else {
        for (size_t i = pow10_cap; i < new_cap; ++i)
            tmp[i] = tmp[i - 1] * (seq_t)10;
    }
    pow10 = tmp;
    pow10_cap = new_cap;
}

/* Grow DP tables to next half‑length m+1 */
static void extend_to_next_m(void)
{
    size_t next_m = cur_m + 1;
    size_t new_range = 9 * next_m;

    seq_t *new_cntL = (seq_t *)calloc(new_range + 1, sizeof(seq_t));
    seq_t *new_sumL = (seq_t *)calloc(new_range + 1, sizeof(seq_t));
    seq_t *new_cntR = (seq_t *)calloc(new_range + 1, sizeof(seq_t));
    seq_t *new_sumR = (seq_t *)calloc(new_range + 1, sizeof(seq_t));
    if (!new_cntL || !new_sumL || !new_cntR || !new_sumR)
        abort();

    for (size_t s = 0; s <= max_range; ++s) {
        seq_t cL = cntL[s], vL = sumL[s];
        seq_t cR = cntR[s], vR = sumR[s];

        /* right part: each digit 0..9 appended */
        for (int d = 0; d <= 9; ++d) {
            size_t ns = s + (size_t)d;
            new_cntR[ns] += cR;
            new_sumR[ns] += vR * (seq_t)10 + cR * (seq_t)d;
        }

        /* left part: first digit cannot be zero when next_m == 1 */
        int d_start = (next_m == 1) ? 1 : 0;
        for (int d = d_start; d <= 9; ++d) {
            size_t ns = s + (size_t)d;
            new_cntL[ns] += cL;
            new_sumL[ns] += vL * (seq_t)10 + cL * (seq_t)d;
        }
    }

    free(cntL); free(sumL); free(cntR); free(sumR);
    cntL = new_cntL; sumL = new_sumL;
    cntR = new_cntR; sumR = new_sumR;
    cur_m = next_m;
    max_range = new_range;
}

/* Ensure cache computed for all n ≤ required */
static void ensure_a_upto(idx_t n)
{
    if (n <= cache_max)
        return;

    ensure_pow10((size_t)n);
    ensure_cache((size_t)n);

    if (cur_m == 0) {
        cntL = (seq_t *)calloc(1, sizeof(seq_t));
        sumL = (seq_t *)calloc(1, sizeof(seq_t));
        cntR = (seq_t *)calloc(1, sizeof(seq_t));
        sumR = (seq_t *)calloc(1, sizeof(seq_t));
        if (!cntL || !sumL || !cntR || !sumR)
            abort();
        cntL[0] = cntR[0] = (seq_t)1;
        sumL[0] = sumR[0] = (seq_t)0;
        max_range = 0;
    }
    if (!cache_a)
        cache_a = (seq_t *)calloc(1, sizeof(seq_t));
    if (cache_max == 0)
        cache_a[0] = (seq_t)0;

    seq_t running = cache_a[cache_max];
    for (idx_t len = cache_max + 1; len <= n; ++len) {
        size_t m = (size_t)(len / 2);
        while (cur_m < m)
            extend_to_next_m();

        seq_t len_sum = (seq_t)0;

        if ((len & 1ULL) == 0) {          /* even length 2*m */
            seq_t p10m = pow10[m];
            for (size_t s = 0; s <= max_range; ++s) {
                seq_t cL = cntL[s], cR = cntR[s];
                seq_t vL = sumL[s], vR = sumR[s];
                if (cL == 0 || cR == 0)
                    continue;
                len_sum += vL * p10m * cR + cL * vR;
            }
        } else {                          /* odd length 2*m+1 */
            seq_t p10m   = pow10[m];
            seq_t p10m1  = pow10[m + 1];
            seq_t C45    = (seq_t)45;
            for (size_t s = 0; s <= max_range; ++s) {
                seq_t cL = cntL[s], cR = cntR[s];
                seq_t vL = sumL[s], vR = sumR[s];
                if (cL == 0 || cR == 0)
                    continue;
                len_sum += vL * p10m1 * cR * 10;
                len_sum += cL * vR * 10;
                len_sum += C45 * p10m * cL * cR;
            }
        }

        running += len_sum;
        cache_a[len] = running;
    }
    cache_max = n;
}

/* --------------------------------------------------------------- */
/* Public function                                                   */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    ensure_a_upto(n);
    return cache_a[n];
}
