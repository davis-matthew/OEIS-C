/*
 * A134739  Cubes of (positive numbers that are not the sum of three nonzero
 *          squares), that is, the terms of A004214, cubed.
 * https://oeis.org/A134739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-th number that is NOT a sum of three * positive
 *            squares)^3 * * The function below builds the sequence on demand.
 *            It keeps a cached * list of the first computed cubes and a table
 *            of which integers can * be written as a sum of two positive
 *            squares. For each candidate * integer m it checks...
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 307, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute integer floor square root using binary search. */
static inline idx_t isqrt_idx(idx_t n) {
    idx_t low = 0, high = n, mid, r;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        r = mid * mid;
        if (r == n) return mid;
        if (r < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Table of numbers that are sums of two positive squares. */
static unsigned char *sq2_table = NULL;   /* 1 if sum of two squares, else 0 */
static idx_t      sq2_size   = 0;          /* array size (limit + 1) */

/* Recompute the table up to at least 'limit' (inclusive). */
static void ensure_sq2_upto(idx_t limit) {
    if (limit + 1 <= sq2_size)
        return;          /* already covered */

    /* Allocate / enlarge the table. */
    idx_t new_cap = limit + 1;
    unsigned char *tmp = (unsigned char *)realloc(sq2_table, (size_t)new_cap * sizeof(unsigned char));
    if (!tmp)
        abort();
    sq2_table = tmp;
    sq2_size  = new_cap;

    /* Re‑initialize the entire table to 0. */
    for (idx_t i = 0; i < limit; ++i)
        sq2_table[i] = 0;

    /* Compute all i^2 + j^2 <= limit, with i,j >= 1. */
    idx_t sqrtN = isqrt_idx(limit);
    for (idx_t i = 1; i <= sqrtN; ++i) {
        idx_t i2 = i * i;
        for (idx_t j = 1; j <= sqrtN; ++j) {
            idx_t s = i2 + j * j;
            if (s > limit)
                break;
            sq2_table[s] = 1;
        }
    }
}

/* Test whether `m` can be expressed as a sum of three positive squares. */
static bool is_three_pos_square(idx_t m) {
    if (m < 3)
        return false;              /* minimal sum is 1^2+1^2+1^2 = 3 */

    /* Ensure the two‑square table is up to date. */
    if (m + 1 > sq2_size)
        ensure_sq2_upto(m + 1000);   /* add a margin to reduce future resizing */

    idx_t a_max = isqrt_idx(m - 2);    /* a^2 ≤ m-2 (since b^2 + c^2 ≥ 1^2+1^2 = 2) */
    for (idx_t a = 1; a <= a_max; ++a) {
        idx_t rem = m - a * a;        /* remaining after fixing a^2 */
        if (rem < 2)                  /* need at least 1^2 + 1^2 */
            continue;
        if (sq2_table[rem])           /* b^2 + c^2 = rem is possible */
            return true;
    }
    return false;
}

/* Cache of computed cubes. */
static seq_t *cache    = NULL;
static idx_t  cache_sz = 0;
static idx_t  cache_cap = 0;

/* Ensure that at least `target` terms are in the cache. */
static void build_until(idx_t target) {
    if (!cache) {
        cache_cap = 32U;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
        cache_sz = 0U;
    }

    static idx_t candidate = 1U;   /* next integer to test */
    while (cache_sz < target) {
        /* Extend two‑square table if necessary. */
        if (candidate + 1 > sq2_size)
            ensure_sq2_upto(candidate + 1000);

        /* If candidate is NOT representable as sum of 3 positive squares,
         * add its cube to the cache. */
        if (!is_three_pos_square(candidate)) {
            seq_t cube = (seq_t)candidate * candidate * candidate;
            if (cache_sz == cache_cap) {
                cache_cap <<= 1;
                seq_t *newc = (seq_t *)realloc(cache, (size_t)cache_cap * sizeof(seq_t));
                if (!newc)
                    abort();
                cache = newc;
            }
            cache[cache_sz++] = cube;
        }
        ++candidate;
    }
}

/* Public function required by the build system. */
seq_t a(idx_t n) {
    if (n < 1U)
        return (seq_t)0U;      /* undefined for n<1, harmless */
    build_until(n);
    return cache[n - 1U];
}
