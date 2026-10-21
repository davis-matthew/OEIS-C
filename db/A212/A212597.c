/*
 * A212597  Number of ways of writing n in the form i*j+k*m with 0<i<=j,
 *          0<k<=m, and (j<m or j=m and i<k).
 * https://oeis.org/A212597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of ways of writing n = i*j + k*m with * 0 < i <= j
 *            , 0 < k <= m and (j < m or (j == m && i < k))
 * Range:     u8 n <= 83, u16 n <= 4919, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct { uint16_t j; uint16_t i; } Pair;

/* Static data used by the memoising builder. */
static idx_t          maxN      = 0;   /* maximum N for which tables exist */
static Pair *         pairs     = NULL;/* all pairs, contiguous */
static size_t *       start     = NULL;/* start index of each product's list */
static size_t *       len       = NULL;/* number of pairs in each product's list */

/* Compare two pairs by (j then i).  Used by qsort. */
static int cmp_pair(const void *p1, const void *p2)
{
    const Pair *a = (const Pair *)p1;
    const Pair *b = (const Pair *)p2;
    if (a->j != b->j) return (int)(a->j - b->j);
    return (int)(a->i - b->i);
}

/* Count in a sorted list of pairs all elements strictly greater than key.
 * The list is [base, base+len). */
static size_t count_greater(const Pair *base, size_t len, const Pair *key)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        const Pair *cur = base + mid;
        if (cur->j < key->j || (cur->j == key->j && cur->i <= key->i))
            lo = mid + 1;
        else
            hi = mid;
    }
    return len - lo;
}

/* Build all factor-pair tables up to N inclusive. Rebuilds if N is larger
 * than the previous maximum. */
static void build_upto(idx_t N)
{
    if (N <= maxN) return;            /* already built enough */

    /* free old tables if any */
    free(pairs); free(start); free(len);
    pairs = NULL; start = NULL; len = NULL;

    /* allocate per-product length and cumulative start arrays */
    start = (size_t *)calloc((size_t)(N + 1), sizeof(size_t));
    len   = (size_t *)calloc((size_t)(N + 1), sizeof(size_t));
    if (!start || !len) return;       /* allocation failed */

    /* First pass: count pairs for each product */
    for (idx_t v = 1; v <= N; ++v) {
        size_t count = 0;
        for (idx_t i = 1; i * i <= v; ++i)
            if (v % i == 0) ++count;   /* i <= sqrt(v) guarantees i <= v/i */
        len[v] = count;
    }

    /* Compute total number of pairs and cumulative start indices */
    size_t total = 0;
    for (idx_t v = 1; v <= N; ++v) {
        start[v] = total;
        total   += len[v];
    }

    /* Allocate the global pair array */
    pairs = (Pair *)malloc(total * sizeof(Pair));
    if (!pairs) return;               /* allocation failed */

    /* Temporary array to track next free slot per product */
    size_t *cur = (size_t *)calloc((size_t)(N + 1), sizeof(size_t));
    if (!cur) return;

    /* Second pass: fill the pairs array */
    for (idx_t v = 1; v <= N; ++v) {
        for (idx_t i = 1; i * i <= v; ++i) {
            if (v % i == 0) {
                size_t pos = start[v] + cur[v];
                pairs[pos].i = (uint16_t)i;
                pairs[pos].j = (uint16_t)(v / i);
                ++cur[v];
            }
        }
    }
    free(cur);

    /* Sort each product's list individually by (j, i) */
    for (idx_t v = 1; v <= N; ++v) {
        if (len[v] > 1)
            qsort(&pairs[start[v]], len[v], sizeof(Pair), cmp_pair);
    }

    maxN = N;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 2) return 0;           /* known small values */

    idx_t needed = n - 1;
    build_upto(needed);             /* ensure tables cover all needed products */

    seq_t total = 0;

    for (idx_t p = 1; p < n; ++p) {
        idx_t q = n - p;
        size_t lp = len[p], lq = len[q];
        if (lp == 0 || lq == 0) continue;

        const Pair *listp = &pairs[start[p]];
        const Pair *listq = &pairs[start[q]];

        for (size_t ip = 0; ip < lp; ++ip) {
            const Pair *a = &listp[ip];
            size_t add = count_greater(listq, lq, a);
            total += (seq_t)add;
        }
    }
    return total;
}
