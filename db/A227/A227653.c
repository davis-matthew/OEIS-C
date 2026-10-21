/*
 * A227653  a(1) = least k such that 1/2 + 1/3 < H(k) - H(3); a(2) = least k
 *          such that H(a(1)) - H(3) < H(k) -H(a(1)), and for n > 2, a(n) =
 *          least k such that H(a(n-1)) - H(a(n-2)) > H(k) - H(a(n-1)), where
 *          H = harmonic number.
 * https://oeis.org/A227653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A227653
 * Defined recursively by
 *   a(1)=8, a(2)=21, a(3)=54, a(4)=138
 *   a(n)=3*a(n-1)-a(n-2)-a(n-4)   for n≥5
 * This recurrence was conjectured for all n and generates the known
 * terms of the OEIS sequence.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Retain a cache of computed terms so that calls are fast and can be
 * in any order.  The cache grows only as needed. */
static seq_t *seq_cache = NULL;   /* array of a(i), 1‑based indices */
static idx_t  seq_known = 0;      /* largest n for which seq_cache[n-1] is valid */
static size_t seq_cap   = 0;      /* allocated capacity of seq_cache */

/* Ensure the cache has room for at least 'need' elements.  The
 * allocation strategy is simple: grow by a factor of two.     */
static void
ensure_cap(size_t need)
{
    if (seq_cap < need) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 1;
        if (new_cap < need) new_cap = need;
        seq_t *new_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();            /* out of memory */
        seq_cache = new_cache;
        seq_cap   = new_cap;
    }
}

/* Return a(n) for n ≥ 1.  The result is exact when it fits in seq_t. */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;                /* undefined; return 0 for safety */

    /* Compute missing terms up to n */
    if ((size_t)n > seq_known) {
        size_t need = (size_t)n;
        ensure_cap(need);

        for (idx_t i = seq_known + 1; i <= n; ++i) {
            seq_t val;
            if (i == 1)          val = 8;
            else if (i == 2)     val = 21;
            else if (i == 3)     val = 54;
            else if (i == 4)     val = 138;
            else {
                seq_t t = seq_cache[(size_t)(i-2)+0] * 3;   /* 3*a(i-1) */
                t -= seq_cache[(size_t)(i-3)+0];           /* -a(i-2) */
                t -= seq_cache[(size_t)(i-5)+0];           /* -a(i-4) */
                val = t;
            }
            seq_cache[(size_t)(i-1)+0] = val;
        }
        seq_known = n;
    }
    return seq_cache[(size_t)(n-1)+0];
}
