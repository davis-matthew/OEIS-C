/*
 * A209491  Table T(n,d), read by antidiagonals, gives the number of subsets
 *          of length n containing an arithmetic progression of length 3 with
 *          distance d.
 * https://oeis.org/A209491
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = read by antidiagonals of the table T(n,d) = 2^n -
 *            ∏_{i=0}^{d-1} Tri(floor((n+i)/d)+2), where Tri(0)=0, Tri(1)=1,
 *            Tri(2)=1, Tri(k)=Tri(k-1)+Tri(k-2)+Tri(k-3) (i.e. the tribonacci
 *            sequence with these initial values)
 * Range:     u8 n <= 30, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* size_t */
#include <stdlib.h> /* realloc, abort */
#include "../utils/pow2.h" /* pow2(exp) returns 2^exp in seq_t */

static seq_t *trib_cache = NULL;   /* stores Tri(k) for k = 0 ... max_index */
static size_t trib_len   = 0;      /* number of Tribonacci values computed (next index to compute) */

/* Ensure that trib_cache contains Tri(k) for all k <= up_to (inclusive). */
static void ensure_tribonacci(size_t up_to)
{
    if (trib_len > up_to) return;          /* already computed */

    /* Allocate memory for trib_cache if necessary */
    if (!trib_cache) {
        trib_cache = (seq_t *)realloc(NULL, (up_to + 1) * sizeof(seq_t));
        if (!trib_cache) abort();
        trib_len = 0;
    } else if (trib_len <= up_to) {
        trib_cache = (seq_t *)realloc(trib_cache, (up_to + 1) * sizeof(seq_t));
        if (!trib_cache) abort();
    }

    /* Compute new Tribonacci values */
    if (trib_len == 0) {
        trib_cache[0] = (seq_t)0;
        trib_cache[1] = (seq_t)1;
        trib_cache[2] = (seq_t)1;
        trib_len = 3;
    }
    for (size_t i = trib_len; i <= up_to; ++i) {
        trib_cache[i] = trib_cache[i-1] + trib_cache[i-2] + trib_cache[i-3];
    }
    trib_len = up_to + 1;
}

/* Compute a single table entry T(n,d) using the given definition. */
static seq_t table_T(idx_t n, idx_t d)
{
    if (d < 1) return (seq_t)0;             /* safety for invalid d */

    /* We need Tri values up to index floor((n+i)/d)+2 for i=0..d-1.
       The maximum value occurs at i = d-1 -> ceil((n+d-1)/d) + 2 <= (n + d-1)/d + 2,
       which is at most floor((n + (d-1))/d) + 2 < (n + d)/d + 2 <= n + 2. */
    ensure_tribonacci((size_t)(n + 2));

    seq_t prod = (seq_t)1;
    for (idx_t i = 0; i < d; ++i) {
        idx_t index = ((n + i) / d) + 2; /* floor((n+i)/d)+2 */
        prod *= trib_cache[(size_t)index];
    }

    seq_t power = pow2(n);                /* 2^n */
    return power - prod;
}

/* Main sequence function entry point */
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;         /* outside the defined offset */

    /* Convert sequence index to zero‑based index inside the table traversal */
    idx_t idx = n - 3;

    /* Find the diagonal level s such that the total number of elements
       up to that diagonal exceeds idx.
       Each diagonal s has length s+1, so cumulative count after s diagonals is
       (s+1)(s+2)/2. */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= idx) ++s;

    /* Number of elements before diagonal s */
    idx_t prev_sum = s * (s + 1) / 2;

    /* Offset within diagonal s (0 <= offset <= s) */
    idx_t offset = idx - prev_sum;

    /* Coordinates inside the table: zero‑based indices i=n-3, j=d-1 */
    idx_t i = s - offset;               /* row offset from the top of the diagonal */
    idx_t j = offset;                   /* column offset from the left of the diagonal */

    /* Convert back to table indices n (starting from 3) and d (starting from 1) */
    idx_t n_table = i + 3;
    idx_t d_table = j + 1;

    return table_T(n_table, d_table);
}
