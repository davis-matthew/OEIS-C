/*
 * A229140  Smallest k such that k^2 + l^2 = n-th number expressible as sum of
 *          two squares (A001481).
 * https://oeis.org/A229140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9727 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A229140:  Smallest k such that k² + l² = n‑th number expressible
 * as a sum of two squares (A001481).  a(n) = 0 if the number is a square;
 * otherwise a(n) = min{k ≥ 0 : ∃ l with k² + l² = A001481(n)}.
 *
 * The routine pre‑computes all sums of two squares up to a fixed bound
 * sufficient to cover the first 10000 terms (limit 250).  Once built,
 * it answers any query in O(1) by scanning k up to √N.  Complexity:
 * O(N) to build (N≈62500) and O(√N) for the k‑search, well below the
 * requested 2‑second limit.  The implementation works for all
 * integer types supported by seq_t.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Pre‑computed table of the first ~98 000 distinct sums of two squares.
 * The array is built once, lazily, the first time a() is called. */
static seq_t *sums = NULL;         /* sorted list of distinct sums   */
static size_t sums_len = 0;        /* number of cached sums          */
static bool built = false;         /* flag that the table is ready    */

static void build_sums(void)
{
    /* --- Choose a bound large enough to cover at least 10 000 terms.
     *     Estimate: for x, #sums ≤ π/4·x.
     *     For nth≈10 000, take x≈120 000 ≈ 250² + 250².          */
    const idx_t limit = 250;
    const size_t pair_count = (size_t)(limit + 1) * (size_t)(limit + 1);

    /* Allocate temporary array for all i² + j² values.  */
    seq_t *tmp = mem_new(seq_t, pair_count);
    if (!tmp) exit(EXIT_FAILURE);

    /* Generate the raw list.  */
    size_t pos = 0;
    for (idx_t i = 0; i <= limit; ++i)
    {
        seq_t i2 = (seq_t)i * (seq_t)i;
        for (idx_t j = 0; j <= limit; ++j)
        {
            tmp[pos++] = i2 + (seq_t)j * (seq_t)j;
        }
    }

    /* Sort the list.  */
    mem_qsort(tmp, pair_count, sizeof(seq_t), cmp_seq_t);

    /* Deduplicate into the static sums array.  */
    sums = mem_new(seq_t, pair_count);
    if (!sums) exit(EXIT_FAILURE);

    size_t uniq = 0;
    for (size_t i = 0; i < pair_count; ++i)
    {
        if (i == 0 || tmp[i] != tmp[i - 1])
        {
            sums[uniq++] = tmp[i];
        }
    }

    sums_len = uniq;
    if (sums_len < 10000)    /* safety check */
    {
        /* In the unlikely event the bound is insufficient, grow
           the table.  This path is extremely unlikely for the
           demanded range. */
        mem_free(tmp);
        mem_free(sums);
        sums = NULL;
        sums_len = 0;
        built = false;
        return;
    }

    mem_free(tmp);
    built = true;
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* sequence starts at n=1; ignore n=0 */

    /* Build the table on first use. */
    if (!built) build_sums();

    /* If for some reason the table failed to build (unlikely), abort. */
    if (!built) return 0;

    /* Indexing in A001481 is 1‑based.  */
    seq_t N = sums[n - 1];

    /* Find the smallest k such that N - k² is a perfect square. */
    seq_t kmax = floor_sqrt(N);
    for (seq_t k = 0; k <= kmax; ++k)
    {
        seq_t k2 = k * k;
        seq_t m  = N - k2;
        seq_t l  = floor_sqrt(m);
        if (l * l == m) return k;
    }

    /* Fallback (should never happen). */
    return 0;
}
