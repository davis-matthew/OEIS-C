/*
 * A186445  The number of partitions of n in which the first part is at least
 *          four times larger than the second part.
 * https://oeis.org/A186445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = p(n)-p(n-2)-p(n-3)-p(n-4)+p(n-5)+p(n-6)+p(n-7)-p(n-9),
 * where p(n) is the integer partition function A000041(n).
 *
 * The routine keeps a static cache of partition numbers up to the largest
 * n requested, extending it only when necessary.  The cache is reused
 * between calls, so any order of calls is supported.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build system */
static seq_t *p_cache = NULL;   /* partition numbers 0 .. p_cap-1   */
static idx_t p_cap   = 0;       /* current allocated capacity       */
static idx_t p_last  = 0;       /* highest index already computed   */

/* Compute partition numbers p(0)..p(upto) into the static cache. */
static void ensure_partitions(idx_t upto)
{
    /* Initialise or enlarge the cache if required. */
    if (!p_cache) {                         /* first call */
        p_cap = upto + 1;
        if (p_cap == 0) p_cap = 1;          /* make room for p(0) */
        p_cache = (seq_t *)malloc(p_cap * sizeof(seq_t));
        if (!p_cache) abort();              /* OOM */
        p_cache[0] = (seq_t)1;              /* p(0) = 1 */
        p_last  = 0;
    } else if (upto >= p_cap) {             /* need more space */
        idx_t new_cap = upto + 1;
        seq_t *new_cache = (seq_t *)realloc(p_cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        p_cache = new_cache;
        p_cap   = new_cap;
    }

    /* If we already calculated up to 'upto', nothing more to do. */
    if (p_last >= upto) return;

    /* Compute partition numbers for indices > p_last up to 'upto'.
     * Uses the classical recurrence with generalized pentagonal numbers. */
    for (idx_t n = p_last + 1; n <= upto; ++n) {
        seq_t sum = 0;
        for (int k = 1; ; ++k) {
            /* g(k) = k(3k-1)/2  and  g'(k) = k(3k+1)/2 */
            idx_t g1 = (idx_t)k * (((idx_t)3 * (idx_t)k) - 1) / 2;
            if (g1 > n) break;
            if (k & 1) sum += p_cache[n - g1];
            else       sum -= p_cache[n - g1];

            idx_t g2 = (idx_t)k * (((idx_t)3 * (idx_t)k) + 1) / 2;
            if (g2 > n) continue;
            if (k & 1) sum += p_cache[n - g2];
            else       sum -= p_cache[n - g2];
        }
        p_cache[n] = sum;
    }
    p_last = upto;
}

/* Public API: number of partitions of n with first part ≥ 4×second part. */
seq_t a(idx_t n)
{
    /* Ensure partition numbers up to n are available. */
    ensure_partitions(n);

    /* Compute a(n) from the cached partition values. */
    seq_t res = p_cache[n];          /* p(n)   */
    if (n >= 2) res -= p_cache[n-2]; /* p(n-2) */
    if (n >= 3) res -= p_cache[n-3]; /* p(n-3) */
    if (n >= 4) res -= p_cache[n-4]; /* p(n-4) */
    if (n >= 5) res += p_cache[n-5]; /* p(n-5) */
    if (n >= 6) res += p_cache[n-6]; /* p(n-6) */
    if (n >= 7) res += p_cache[n-7]; /* p(n-7) */
    if (n >= 9) res -= p_cache[n-9]; /* p(n-9) */
    return res;
}
