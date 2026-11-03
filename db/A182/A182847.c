/*
 * A182847  Joint-rank array of the numbers j*(i-1+r), where r=sqrt(3), i>=1,
 *          j>=1, by diagonals.
 * https://oeis.org/A182847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value of the joint‑rank array T(i,j) on the * n‑th
 *            anti‑diagonal, where * T(i,j) = Σ_{k≥1} ⌊j*(i‑1+√3)/(k‑1+√3)⌋ *
 *            i,j ≥ 1 * The sequence enumerates T(i,j) by diagonals (i+j
 *            constant)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* ----------  Diagonal lookup helpers  ---------- */
static idx_t diag_of_n(idx_t n)
{
    idx_t lo = 1, hi = 1;
    while (hi * (hi + 1) / 2 < n) hi <<= 1;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (mid * (mid + 1) / 2 >= n) hi = mid;
        else                           lo = mid + 1;
    }
    return lo;                                   /* smallest d with d(d+1)/2 >= n */
}

/* ----------  Fixed‑point representation of √3  ---------- */
typedef unsigned __int128 u128;

static const unsigned SCALE_BITS = 60;
static const u128 SCALE = ((u128)1) << SCALE_BITS;
static u128 SQRT3_NUM;

/* Integer square‑root for 128‑bit values */
static u128 sqrt_floor_u128(u128 x)
{
    u128 lo = 0, hi = ((u128)1) << 64;          /* sqrt(2^128‑1) < 2^64 */
    while (lo <= hi) {
        u128 mid = (lo + hi) >> 1;
        u128 sq  = mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* Initialise √3 scaled integer once */
static void init_constants(void)
{
    static int done = 0;
    if (done) return;
    done = 1;
    SQRT3_NUM = sqrt_floor_u128((u128)3 * SCALE * SCALE);
}

/* ---------  Compute T(i,j)  --------- */
static seq_t compute_T(idx_t i, idx_t j)
{
    init_constants();
    u128 base  = (u128)(i-1) * SCALE + SQRT3_NUM;     /* (i-1)+√3 in FP */
    u128 num_fp = (u128)j * base;                    /* j*(i-1+√3) */
    u128 den_fp = SQRT3_NUM;                          /* (k-1)+√3 for k=1 */
    seq_t sum = 0;

    while (1) {
        seq_t v = (seq_t)(num_fp / den_fp);
        if (v == 0) break;
        sum += v;
        den_fp += SCALE;                             /* k increases by 1 */
    }
    return sum;
}

/* ----------  Sequence cache  ---------- */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_size = 0;
static idx_t diag_done = 0;

/* Grow cache capacity */
static void grow_cache(void)
{
    if (cache_cap == 0)      cache_cap = 16;
    else                      cache_cap <<= 1;
    cache = mem_resize(seq_t, cache, cache_cap);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1 – undefined for n=0 */

    if (n <= cache_size) return cache[n-1];

    /* Need to extend cache up to n */
    idx_t need_diagonal = diag_of_n(n);
    for (idx_t d = diag_done + 1; d <= need_diagonal; ++d) {
        for (idx_t i = 1; i <= d; ++i) {
            idx_t j = d + 1 - i;
            seq_t val = compute_T(i, j);
            if (cache_size == cache_cap) grow_cache();
            cache[cache_size++] = val;
        }
    }
    diag_done = need_diagonal;
    return cache[n-1];
}
