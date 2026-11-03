/*
 * A050241  Handsome numbers (A007532) representable in exactly two distinct
 *          ways (counting different powers of duplicated digits as distinct).
 * https://oeis.org/A050241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Enumeration: generates candidate numbers and counts representations
    that give exactly two distinct ways of writing the number as a sum
    of digit^exponent for each decimal digit in the number.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Depth‑first search counting representations. */
static void dfs_rep(int idx, int k, seq_t sum, seq_t target,
                    seq_t (*powlist)[130], int *len,
                    seq_t minRem[21], seq_t maxRem[21], int *cnt)
{
    if (*cnt > 2) return;                 /* early stop */
    if (idx == k) {
        if (sum == target) ++(*cnt);
        return;
    }
    /* pruning */
    if (sum + minRem[idx] > target) return;
    if (sum + maxRem[idx] < target) return;

    seq_t *plist = powlist[idx];
    int l = len[idx];
    for (int j = 0; j < l; ++j) {
        seq_t p = plist[j];
        if (sum + p > target) break;
        dfs_rep(idx + 1, k, sum + p, target,
                powlist, len, minRem, maxRem, cnt);
        if (*cnt > 2) return;
    }
}

/* Test whether n is a Handsome number (exactly two representations). */
static bool is_handsome(seq_t n)
{
    /* Extract digits most‑significant first. */
    seq_t tmp = n;
    int digs_rev[20];
    int k = 0;
    if (tmp == 0) return false;
    while (tmp > 0) {
        digs_rev[k++] = (int)(tmp % 10);
        tmp /= 10;
    }
    int digs[20];
    for (int i = 0; i < k; ++i) digs[i] = digs_rev[k-1-i];

    /* Build possible powers for each digit position. */
    seq_t powlist[20][130];
    int len[20];
    const int MAX_POW = 130;
    for (int i = 0; i < k; ++i) {
        int d = digs[i];
        if (d == 0) {                     /* 0^any = 0 */
            powlist[i][0] = 0;
            len[i] = 1;
            continue;
        }
        if (d == 1) {                     /* treat 1 as a single value 1 */
            powlist[i][0] = 1;
            len[i] = 1;
            continue;
        }
        seq_t cur = d;
        int l = 0;
        while (cur <= n) {
            if (l >= MAX_POW) break;
            powlist[i][l++] = cur;
            /* guard against overflow */
            if (cur > n / d) break;
            cur *= d;
        }
        len[i] = l;
        if (l == 0) return false;         /* should not happen */
    }

    /* Pre‑compute min and max remaining sums for pruning. */
    seq_t minRem[21] = {0};
    seq_t maxRem[21] = {0};
    for (int i = k-1; i >= 0; --i) {
        minRem[i] = minRem[i+1] + powlist[i][0];
        seq_t maxp = powlist[i][len[i]-1];
        maxRem[i]  = maxRem[i+1] + maxp;
    }

    int cnt = 0;
    dfs_rep(0, k, 0, n, powlist, len, minRem, maxRem, &cnt);
    return cnt == 2;
}

/* Grow an array of seq_t elements. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_cap)
{
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr   = tmp;
    *size  = new_cap;
}

/* Storage for the sequence (1‑based). */
static seq_t *handsome_arr = NULL;
static size_t handsome_len = 0;
static size_t handsome_cap = 0;

/* Ensure the sequence contains at least `upto` terms (1‑based). */
static void compute_up_to(idx_t upto)
{
    if (!handsome_arr) {
        handsome_cap = 256;
        handsome_arr = mem_new(seq_t, handsome_cap);
        if (!handsome_arr) abort();
    }

    seq_t cand = 1;
    if (handsome_len > 0) cand = handsome_arr[handsome_len-1] + 1;

    while ((idx_t)handsome_len < upto) {
        if (is_handsome(cand)) {
            if (handsome_len == handsome_cap) {
                handsome_cap <<= 1;
                grow_seq_array(&handsome_arr, &handsome_cap, handsome_cap);
            }
            handsome_arr[handsome_len++] = cand;
        }
        ++cand;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset is 1, behaviour for n<1 undefined */
    compute_up_to(n);
    return handsome_arr[n-1];
}
