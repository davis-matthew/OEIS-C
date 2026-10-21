/*
 * A216500  Numbers which are simultaneously of the form x^2+y^2, x^2+2y^2,
 *          x^2+3y^2, x^2+7y^2, all with x>=0, y>=0.
 * https://oeis.org/A216500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n‑th positive integer that can be written in all four 
 * forms x² + y², x² + 2y², x² + 3y² and x² + 7y² for some non‑negative integers x, y.
 * The function builds an incremental sieve over the first N integers, marking
 * which are representable by each quadratic form, then extracts the numbers that
 * are marked by all four.  For each call it expands the sieve if necessary.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------- helpers ---------- */

/* integer square root for 64‑bit values, safe for all idx_t values            */
static inline idx_t _isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t low  = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == (unsigned __int128)n) return mid;
        if (sq < (unsigned __int128)n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* ------- global state ---------- */

static uint8_t *mask1 = NULL;
static uint8_t *mask2 = NULL;
static uint8_t *mask3 = NULL;
static uint8_t *mask4 = NULL;
static size_t   mask_len = 0;          /* size of each mask array */

/* cached values of the sequence */
static seq_t *terms   = NULL;
static size_t terms_len = 0;           /* number of terms stored     */
static size_t terms_cap = 0;           /* allocated capacity of terms[] */

/* ------- rebuild routine ---------- */

static void _rebuild_masks_and_terms(size_t limit)
{
    size_t sz = limit + 1;

    /* free old masks if any */
    free(mask1); free(mask2); free(mask3); free(mask4);

    /* allocate new zeroed masks */
    mask1 = (uint8_t*)calloc(sz, sizeof(uint8_t));
    mask2 = (uint8_t*)calloc(sz, sizeof(uint8_t));
    mask3 = (uint8_t*)calloc(sz, sizeof(uint8_t));
    mask4 = (uint8_t*)calloc(sz, sizeof(uint8_t));

    mask_len = limit;

    /* masks for the four quadratic forms x² + k·y² */
    const int ks[4]   = {1, 2, 3, 7};
    uint8_t *masks[4] = {mask1, mask2, mask3, mask4};

    for (int kdx = 0; kdx < 4; ++kdx) {
        int k = ks[kdx];
        idx_t max_y = _isqrt_u64(limit / k);
        for (idx_t y = 0; y <= max_y; ++y) {
            idx_t y2  = y * y;
            idx_t ky2 = (idx_t)k * y2;
            if (ky2 > limit) break;
            idx_t rem = limit - ky2;
            idx_t max_x = _isqrt_u64(rem);
            for (idx_t x = 0; x <= max_x; ++x) {
                idx_t val = x * x + ky2;
                masks[kdx][val] = 1;
            }
        }
    }

    /* free old terms if any */
    free(terms); terms = NULL; terms_len = 0;
    /* rough capacity estimate: at least 10% density */
    terms_cap = limit / 10 + 10;
    terms = (seq_t*)calloc(terms_cap, sizeof(seq_t));

    /* collect numbers representable by all four forms */
    for (idx_t v = 1; v <= limit; ++v) {
        if (mask1[v] && mask2[v] && mask3[v] && mask4[v]) {
            if (terms_len == terms_cap) {
                terms_cap <<= 1;          /* double capacity */
                terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)v;
        }
    }
}

/* ------- public function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, return 0 safely */

    /* persistent state */
    static size_t curr_limit  = 0;

    /* first call – build an initial sieve           */
    if (curr_limit == 0) {
        curr_limit = 200000;        /* generous start for first ~10 k terms   */
        _rebuild_masks_and_terms(curr_limit);
    }

    /* expand limit until we have at least n terms */
    while (n > terms_len) {
        size_t new_limit = curr_limit * 2;
        if (new_limit < curr_limit + 200000)    /* avoid tiny jumps          */
            new_limit = curr_limit + 200000;
        curr_limit = new_limit;
        _rebuild_masks_and_terms(curr_limit);
    }

    return terms[n - 1];
}
