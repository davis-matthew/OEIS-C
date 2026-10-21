/*
 * A237047  Number of compositions of n minus the number of overpartitions of
 *          n.
 * https://oeis.org/A237047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^(n-1) - O(n) where O(n) is the number of overpartitions
 *            of n (OEIS A015128)
 * Range:     i8 n <= 1, i16 n <= 1, i32 n <= 7, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   pow2, seq_alloc_new_array, seq_free_array, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses dynamic programming.  For each call
   the routine expands the cached arrays up to the requested index.
   p[n]   = number of partitions of n          (OEIS A000041)
   d[n]   = number of partitions into distinct parts (OEIS A000009)
   over[n] = number of overpartitions of n   (convolution of p and d) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/pow2.h"         /* seq_t pow2(idx_t); */
#include "../utils/seq_alloc_new_array.h"  /* seq_t *seq_alloc_new_array(size_t); */
#include "../utils/seq_free_array.h"       /* void seq_free_array(seq_t *); */
#include "../utils/seq_mul.h"      /* seq_t seq_mul(seq_t a, seq_t b); */

static seq_t *p_cache = NULL;     /* partitions p(n)   */
static seq_t *d_cache = NULL;     /* distinct-partitions d(n) */
static seq_t *over_cache = NULL;  /* overpartitions O(n) */
static idx_t cached_size = 0;     /* number of entries cached (>=1) */

/* Extend the cached tables up to the given index n (inclusive) */
static void
ensure_capacity(idx_t n)
{
    if (n + 1 <= cached_size) /* already have enough data */
        return;

    /* Allocate new storage */
    idx_t size = n + 1;
    seq_t *new_p = seq_alloc_new_array(size);
    seq_t *new_d = seq_alloc_new_array(size);
    seq_t *new_over = seq_alloc_new_array(size);

    /* ---- partitions p[n] --------------------------------------- */
    new_p[0] = 1;                 /* p(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            seq_t term = new_p[i - pent1];
            if (k & 1) sum += term;   /* odd k: + */
            else       sum -= term;   /* even k: - */

            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= i) {
                term = new_p[i - pent2];
                if (k & 1) sum += term;
                else       sum -= term;
            }
        }
        new_p[i] = sum;
    }

    /* ---- distinct-partitions d[n] (each part at most once) ---- */
    new_d[0] = 1;                 /* d(0) */
    for (idx_t m = 1; m <= n; ++m) {
        for (idx_t i = n; i >= m; --i) {
            new_d[i] = new_d[i] + new_d[i - m];
        }
    }

    /* ---- overpartitions O[n] = convolution of p and d ---------- */
    new_over[0] = new_p[0] * new_d[0];   /* equals 1 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= i; ++k) {
            seq_t prod = seq_mul(new_p[k], new_d[i - k]);
            sum += prod;
        }
        new_over[i] = sum;
    }

    /* Free old caches if any */
    if (p_cache)   seq_free_array(p_cache);
    if (d_cache)   seq_free_array(d_cache);
    if (over_cache)seq_free_array(over_cache);

    /* Install new caches */
    p_cache = new_p;
    d_cache = new_d;
    over_cache = new_over;
    cached_size = size;
}

seq_t a(idx_t n)
{
    /* Ensure caches contain values up to n */
    ensure_capacity(n);

    /* Number of compositions of n: 1 for n=0, else 2^(n-1) */
    seq_t comps;
    if (n == 0)
        comps = 1;
    else
        comps = pow2(n - 1);

    /* a(n) = compositions - overpartitions */
    return comps - over_cache[n];
}
