/*
 * A122873  Triangle T(n,k) = smallest number whose square has largest digit k
 *          in base n, 0<=k<n.
 * https://oeis.org/A122873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, tabl
 * Formula:   a(n) = smallest integer x such that the largest digit of x^2 in
 *            base * n equals the column index k (0‑based) of a(n) within its
 *            row in the triangle * T(n,k). The function searches x
 *            incrementally, caching results per base
 * Range:     u8 n <= 39, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 91 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* seq_mul is provided by the build. */
#include <string.h>
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* ---------- find base and row offset for a linear index s = n-1 ---------- */
static void find_base(idx_t s, idx_t *base, idx_t *offset)
{
    idx_t low = 1, high = s + 1;   /* base <= s+1 */
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        __uint128_t val = (__uint128_t)mid * (mid - 1) / 2;
        if (val <= s) low = mid;
        else          high = mid - 1;
    }
    *base = low;
    *offset = (idx_t)(*base) * ((*base) - 1) / 2;
}

/* ---------- largest digit of val in base base; base fits in idx_t ---------- */
static inline seq_t max_digit(seq_t val, idx_t base)
{
    seq_t max = 0;
    const seq_t b = (seq_t)base;
    while (val > 0) {
        seq_t d = val % b;
        if (d > max) max = d;
        val /= b;
    }
    return max;
}

/* ---------- row cache structure ---------- */
typedef struct {
    seq_t *values;   /* values for digits 0 .. base-1 */
    bool  *filled;   /* flag if value has been found */
    idx_t  remaining;/* how many digits remain to find */
    idx_t  next_x;    /* next candidate x to test */
} RowCache;

/* ---------- dynamic array of row caches ---------- */
static RowCache **row_cache = NULL;
static size_t cache_capacity = 0;

/* Ensure capacity for at least (base+1) rows (1‑based). */
static void ensure_row_capacity(idx_t base)
{
    if (base + 1 <= cache_capacity) return;
    size_t new_cap = base + 1;
    RowCache **new_cache = (RowCache**)realloc(row_cache, new_cap * sizeof(RowCache *));
    if (!new_cache) exit(EXIT_FAILURE);
    for (size_t i = cache_capacity; i < new_cap; ++i)
        new_cache[i] = NULL;
    row_cache = new_cache;
    cache_capacity = new_cap;
}

/* Retrieve or allocate cache for a given base. */
static RowCache *get_row_cache(idx_t base)
{
    ensure_row_capacity(base);
    if (!row_cache[base]) {
        RowCache *rc = (RowCache*)calloc(1, sizeof(RowCache));
        if (!rc) exit(EXIT_FAILURE);
        rc->values   = mem_new(seq_t, base);
        rc->filled   = (bool*)calloc(base, sizeof(bool));
        rc->remaining = base;      /* all digits still unknown */
        rc->next_x = 0;            /* start testing from 0 */
        row_cache[base] = rc;
    }
    return row_cache[base];
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe guard */

    /* Decompose linear index into base (row) and digit (column) */
    idx_t s = n - 1;
    idx_t base, offset;
    find_base(s, &base, &offset);
    idx_t k = s - offset;          /* column index (0‑based) */

    /* Base 1 is a trivial case: only digit 0 exists. */
    if (base == 1) return 0;

    /* Ensure row cache exists */
    RowCache *rc = get_row_cache(base);

    /* If already computed, return it. */
    if (rc->filled[k]) return rc->values[k];

    /* Scan successive x until digit k is found (and fill others). */
    for (idx_t x = rc->next_x; ; ++x) {
        seq_t sq = seq_mul((seq_t)x, (seq_t)x);   /* x^2      */
        seq_t dgt = max_digit(sq, base);          /* largest digit of sq in base */

        idx_t d = (idx_t)dgt;
        if (!rc->filled[d]) {
            rc->values[d] = x;
            rc->filled[d] = true;
            rc->remaining--;
        }

        /* Update next_x for future calls */
        rc->next_x = x + 1;

        if (rc->filled[k]) return rc->values[k];
        if (rc->remaining == 0) return rc->values[k];
    }
}
