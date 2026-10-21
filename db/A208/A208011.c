/*
 * A208011  Number of nX6 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary matrices avoiding horizontal
 *            patterns 000 and 101, and vertical patterns 001 and 011. For
 *            each parity (odd/even rows) the matrix must satisfy that every
 *            row is a bitwise subset of the row two positions earlier. Let M
 *            be the set of 6‑bit masks that contain no horizontal...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 39, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Lookup tables for valid row masks and for subsets of those masks.   */

static uint8_t  mask_vals[64];        /* indices of valid masks          */
static size_t   mask_cnt;             /* number of valid masks           */
static uint8_t  subset_of[64][64];    /* indices of subsets of each mask */
static uint8_t  subset_cnt[64];       /* number of subsets per mask      */

static bool init_masks_done = false;

static void init_masks(void)
{
    /* find all valid 6‑bit masks (horizontal constraints) */
    mask_cnt = 0;
    for (uint32_t m = 0; m < 64; ++m) {
        bool ok = true;
        for (int i = 0; i < 4; ++i) {
            int b0 = (m >> i)       & 1;
            int b1 = (m >> (i + 1)) & 1;
            int b2 = (m >> (i + 2)) & 1;
            int pat = (b0 << 2) | (b1 << 1) | b2; /* 3‑bit pattern */
            if (pat == 0 || pat == 5) { /* 000 or 101 forbidden */
                ok = false;
                break;
            }
        }
        if (ok) {
            mask_vals[mask_cnt] = (uint8_t)m;
            ++mask_cnt;
        }
    }

    /* precompute subset lists for each mask */
    for (size_t i = 0; i < mask_cnt; ++i) {
        uint8_t a = mask_vals[i];
        subset_cnt[i] = 0;
        for (size_t j = 0; j < mask_cnt; ++j) {
            uint8_t b = mask_vals[j];
            /* b is subset of a iff (b | a) == a   or   (b & ~a) == 0 */
            if (((uint32_t)b | (uint32_t)a) == a) {
                subset_of[i][subset_cnt[i]++] = (uint8_t)j;
            }
        }
    }

    init_masks_done = true;
}

/* ------------------------------------------------------------------ */
/* Dynamic programming state                                         */

static seq_t *chains_total  = NULL;   /* chains_total[L]   0..cur_len */
static size_t chains_len   = 0;      /* number of computed lengths */

static seq_t *dp_cur       = NULL;   /* current DP array for length L */
static seq_t *dp_next      = NULL;   /* temporary DP array          */
static size_t dp_size      = 0;      /* size of dp arrays (mask_cnt) */

/* Extend DP tables to at least length 'need' (>=0) */
static void extend_to(size_t need)
{
    if (!init_masks_done) init_masks();

    /* initialise storage for DP arrays if not yet */
    if (dp_size < mask_cnt) {
        dp_size = mask_cnt;
        dp_cur  = (seq_t *)malloc(dp_size * sizeof(seq_t));
        dp_next = (seq_t *)malloc(dp_size * sizeof(seq_t));
    }

    /* initialise length 0 */
    if (chains_total == NULL) {
        chains_total = (seq_t *)malloc(sizeof(seq_t));
        chains_total[0] = (seq_t)1; /* empty sequence */
        chains_len = 0;
    }

    /* compute length 1 (if needed) */
    if (chains_len < 1) {
        for (size_t i = 0; i < mask_cnt; ++i) dp_cur[i] = (seq_t)1;
        seq_t sum = (seq_t)0;
        for (size_t i = 0; i < mask_cnt; ++i) sum += dp_cur[i];
        chains_total = (seq_t *)realloc(chains_total, (chains_len + 2) * sizeof(seq_t));
        chains_total[1] = sum;
        ++chains_len;
    }

    /* extend step by step up to 'need' */
    while (chains_len < need) {
        size_t next_len = chains_len + 1;
        /* zero dp_next */
        for (size_t i = 0; i < mask_cnt; ++i) dp_next[i] = (seq_t)0;

        /* transition: for each previous mask i, add its count to all subsets */
        for (size_t i = 0; i < mask_cnt; ++i) {
            seq_t w = dp_cur[i];
            if (w == (seq_t)0) continue;
            for (uint8_t j = 0; j < subset_cnt[i]; ++j) {
                size_t idx = subset_of[i][j];
                dp_next[idx] += w;
            }
        }

        /* compute total for this length */
        seq_t sum = (seq_t)0;
        for (size_t i = 0; i < mask_cnt; ++i) sum += dp_next[i];

        /* store total */
        chains_total = (seq_t *)realloc(chains_total,
                                       (chains_len + 2) * sizeof(seq_t));
        chains_total[next_len] = sum;

        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;

        ++chains_len;
    }
}

/* ------------------------------------------------------------------ */
/* Public interface                                                     */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; undefined for 0 */

    /* required chain lengths for odd and even rows */
    idx_t odd_len  = (n + 1) / 2;   /* ceil(n/2)   */
    idx_t even_len = n / 2;         /* floor(n/2)  */

    size_t need = odd_len > even_len ? odd_len : even_len;
    extend_to(need);

    seq_t res = chains_total[odd_len] * chains_total[even_len];
    return res;
}
