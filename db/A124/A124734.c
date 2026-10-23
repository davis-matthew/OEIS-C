/*
 * A124734  Table with all compositions sorted first by total, then by length
 *          and finally lexicographically.
 * https://oeis.org/A124734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 8851, u16 n <= 8851, u32 n <= 8851, u64 n <= 8851,
 *            u128 n <= 8851, big any n
 * Helpers:   elements_for_total
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A124734: the n‑th element in the flattened list of all
   integer compositions sorted by total sum, then by length, then
   lexicographically.  The algorithm finds the sum T, the composition
   length k, and the particular composition using combinatorial
   counting.  It works for any n ≥ 1. */
#include <stdint.h>
#include "../utils/elements_for_total.h"

/* Pre–compute binomial coefficients up to 64, sufficient for n ≤ 10000.
   binom[n][k] = C(n,k) stored as idx_t. */
#define MAX_BINOM 64
static idx_t binom[MAX_BINOM + 1][MAX_BINOM + 1] = {0};
static int binom_inited = 0;

static void init_binom(void)
{
    if (binom_inited) return;
    for (idx_t n = 0; n <= MAX_BINOM; ++n) {
        binom[n][0] = binom[n][n] = (idx_t)1;
        for (idx_t k = 1; k < n; ++k)
            binom[n][k] = binom[n-1][k-1] + binom[n-1][k];
    }
    binom_inited = 1;
}

/* Return the composition of sum t and length k at index idx (0‑based)
   in lexicographic order.  The result is written into out[0..k-1] as
   idx_t values. */
static void get_comp(idx_t t, idx_t k, idx_t idx, idx_t *out)
{
    idx_t remSum = t;
    idx_t remParts = k;
    for (idx_t pos = 0; pos < k; ++pos) {
        if (remParts == 1) {
            out[pos] = remSum;
            break;
        }
        idx_t maxV = remSum - (remParts - 1);      /* leave at least 1 each */
        for (idx_t v = 1; v <= maxV; ++v) {
            idx_t restSum = remSum - v;
            idx_t remRem = remParts - 1;
            idx_t cnt = 0;
            if (restSum >= remRem) {
                cnt = binom[restSum - 1][remRem - 1];
            }
            if (idx < cnt) {
                out[pos] = v;
                remSum -= v;
                remParts -= 1;
                break;
            } else {
                idx -= cnt;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for offset = 1 */
    init_binom();

    /* Find the total sum T that contains index n. */
    idx_t t = 1;
    idx_t cum = 0;
    while (1) {
        idx_t e = elements_for_total(t);
        if (cum + e >= n) break;
        cum += e;
        ++t;
    }

    /* Position within the group of sum t. */
    idx_t pos = n - cum;           /* 1‑based position within sum t */

    /* Iterate over lengths k. */
    for (idx_t k = 1; k <= t; ++k) {
        idx_t comp_cnt = binom[t - 1][k - 1];   /* compositions of length k */
        idx_t group_size = comp_cnt * k;        /* entries contributed */

        if (pos > group_size) {
            pos -= group_size;
            continue;
        }

        /* Locate the specific composition and part. */
        idx_t comp_idx = (pos - 1) / k;      /* 0‑based composition index */
        idx_t part_idx = (pos - 1) % k;      /* 0‑based position within it */

        idx_t comp[MAX_BINOM + 1];
        get_comp(t, k, comp_idx, comp);
        return (seq_t)comp[part_idx];
    }

    /* Should never be reached for valid n. */
    return (seq_t)0;
}
