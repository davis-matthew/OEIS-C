/*
 * A093578  Take n sheets of paper, arrange them into piles, write on each
 *          sheet the cardinality (number of sheets) of its pile. Do this
 *          again, so each sheet is labeled by an ordered pair of positive
 *          integers. How many ways can this be done so that every sheet has a
 *          unique label? (Only distinct sets of label...
 * https://oeis.org/A093578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of sets {k1,…,km} of distinct POSITIVE integers
 *            such that * n = Σ T_{k_i} where T_k = k(k+1)/2 and for any
 *            consecutive indices k_i < k_{i+1} * we have k_{i+1} ≥ 2*k_i.
 *            Each such set corresponds to one admissible labeling *
 *            arrangement. a(0) = 1 by convention (empty set)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static seq_t *counts = NULL;   /* cache for computed a(n) values */
static size_t  counts_size = 0;/* current allocated size of counts array */

/* back‑tracking generation of all admissible sums of triangular numbers */
static void dfs(size_t idx, size_t sum, const seq_t *tri,
                size_t kmax, size_t nmax, seq_t *cnt)
{
    for (size_t i = idx; i <= kmax; ++i) {
        size_t new_sum = sum + (size_t)tri[i];
        if (new_sum <= nmax) {
            ++cnt[new_sum];                 /* one more representation */
            dfs(2 * i, new_sum, tri, kmax, nmax, cnt);
        }
    }
}

/* build counts[0..nmax] – recomputes from scratch for each requested nmax */
static void build_counts(idx_t nmax)
{
    /* if already built up to this point, do nothing */
    if (counts_size > 0 && (idx_t)counts_size - 1 >= nmax)
        return;

    /* triangular numbers up to nmax */
    size_t k = 1;
    size_t tri_cnt = 0;
    while (1) {
        idx_t T = (idx_t)k * (k + 1) / 2;
        if (T > nmax) break;
        ++tri_cnt;
        ++k;
    }

    /* allocate triangular array: index 0 unused for easier 1‑based logic */
    seq_t *tri = (seq_t*)calloc(tri_cnt + 1, sizeof(seq_t));
    if (!tri) { /* allocation failed – leave old cache untouched */ return; }

    for (size_t i = 1; i <= tri_cnt; ++i)
        tri[i] = ((seq_t)i * (i + 1)) / 2;

    /* allocate counts array and reset */
    if (counts) free(counts);
    counts = (seq_t*)calloc((size_t)nmax + 1, sizeof(seq_t));
    if (!counts) { free(tri); return; }
    counts_size = (size_t)nmax + 1;

    /* do not count the empty representation yet */
    dfs(1, 0, tri, tri_cnt, (size_t)nmax, counts);

    /* explicit value for n = 0 */
    counts[0] = (seq_t)1;

    free(tri);   /* triangular numbers no longer needed */
}

seq_t a(idx_t n)
{
    build_counts(n);
    return counts[(size_t)n];
}
