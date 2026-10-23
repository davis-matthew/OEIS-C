/*
 * A026262  Beginning with the natural numbers, swap [ k*sqrt(5) ] and [
 *          (k/4)*(5 + sqrt(5)) ], for all k >= 1.
 * https://oeis.org/A026262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   swap floor(k*sqrt(5)) and floor(k*(5+sqrt(5))/4) for all k>=1.
 *            The sequence is the involutive permutation defined by these
 *            Beatty sequences. The array perm stores a(n)=mapped value for
 *            each occupied index; it grows lazily as more terms are requested
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   max3_idx, max_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/max3_idx.h"
#include "../utils/max_idx.h"

/* integer square root of a 128‑bit value (floor sqrt). */
static inline idx_t isqrt_u128(__int128 x) {
    idx_t low = 0, high = (idx_t)~0ULL;
    if (x <= (__int128)high * high) {
        /* if value fits in 64 bits, narrow upper bound to avoid overflow */
        high = (idx_t)((((__int128)high) * high) > x ? high : (idx_t)high);
    }
    idx_t res = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return res;
}

/* helpers for maximum of two or three values.  No macro to avoid name clashes. */

/* static cache for the permutation */
static seq_t *perm = NULL;      /* array a(1)..a(n) */
static size_t perm_cap = 0;     /* allocated size (index 0 unused) */
static idx_t last_k = 0;        /* last processed k */

/* enlarge the array to hold at least indices 0..idx */
static void enlarge_to(size_t idx) {
    if (idx < perm_cap) return;
    size_t new_cap = idx + 1;
    perm = (seq_t *)realloc(perm, new_cap * sizeof(seq_t));
    for (size_t i = perm_cap; i < new_cap; ++i) perm[i] = 0;
    perm_cap = new_cap;
}

/* compute floor(k*sqrt(5)) as floor(sqrt(5*k*k)) */
static idx_t floor_k_sqrt5(idx_t k) {
    return isqrt_u128((__int128)5 * k * k);
}

/* compute floor(k*(5+sqrt(5))/4) = (5*k + floor(k*sqrt5))/4 */
static idx_t floor_k_kasym(idx_t k) {
    idx_t s = floor_k_sqrt5(k);
    return (5 * k + s) / 4;
}

/* generate the next pair (x,y) from k and store it in the table */
static void generate_next(void) {
    ++last_k;
    idx_t x = floor_k_sqrt5(last_k);
    idx_t y = floor_k_kasym(last_k);
    enlarge_to(max3_idx(x, y, last_k));
    perm[x] = (seq_t)y;
    perm[y] = (seq_t)x;
}

/* ensure the mapping for index n is available */
static void ensure_perm(idx_t n) {
    if (perm == NULL) enlarge_to(1);   /* initial table, index 0 unused */
    enlarge_to((size_t)n);            /* need room for n itself */
    while (perm[n] == 0) generate_next();
}

/* public function required by the platform */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* safety for indices below offset */
    ensure_perm(n);
    return perm[(size_t)n];
}
