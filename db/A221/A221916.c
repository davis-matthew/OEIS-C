/*
 * A221916  Array of products of the list entries of the combinations of n,
 *          taken in reverse standard order.
 * https://oeis.org/A221916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the product of the elements not appearing in the
 *            combination of r chosen from {1..r} corresponding to the k-th
 *            entry in standard order where r = floor(log2(n+1)) and k = n -
 *            (2^r-1). For the first element of each row the product is r!
 *            (the empty subset)
 * Range:     u8 n <= 62, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* factorial cache */
static seq_t *fact_cache = NULL;
static idx_t fact_cache_size = 0;

/* ensure fact_cache contains factorials up to r (inclusive) */
static void ensure_factorials(idx_t r)
{
    if (fact_cache_size <= r) {
        idx_t new_size = r + 1;
        seq_t *new_cache = (seq_t *)realloc(fact_cache,
                                            new_size * sizeof(seq_t));
        if (new_cache == NULL) {
            /* allocation failed; abort */
            exit(EXIT_FAILURE);
        }
        /* compute missing factorials */
        for (idx_t i = fact_cache_size; i <= r; ++i) {
            if (i == 0)
                new_cache[i] = (seq_t)1;
            else
                new_cache[i] = new_cache[i - 1] * (seq_t)i;
        }
        fact_cache = new_cache;
        fact_cache_size = new_size;
    }
}

/* small binomial coefficient for n,k <= 64 using uint64_t */
static uint64_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    uint64_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

/* unrank the subset of size m for the given offset within the
   combinations of r elements in standard order.  The subset is
   stored in out[0..len-1] (sorted increasingly) and its length is
   returned via *len. */
static void unrank_subset(idx_t r, idx_t offset, idx_t out[], idx_t *len)
{
    /* find m such that sum_{i=0}^{m-1} C(r,i) <= offset < sum_{i=0}^{m} C(r,i) */
    idx_t m = 0;
    uint64_t cum = 0;
    for (idx_t k = 0; k <= r; ++k) {
        uint64_t c = binom_small(r, k);
        if (cum + c > offset) {
            m = k;
            break;
        }
        cum += c;
    }
    idx_t remaining = offset - cum;          /* rank within subsets of size m */

    /* generate lexicographic combination */
    idx_t current = 1;
    for (idx_t pos = 0; pos < m; ++pos) {
        for (idx_t cand = current; cand <= r - (m - pos) + 1; ++cand) {
            uint64_t c = binom_small(r - cand, m - pos - 1);
            if (c <= remaining) {
                remaining -= c;
            } else {
                out[pos] = cand;
                current = cand + 1;
                break;
            }
        }
    }
    *len = m;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* determine row r and offset k within that row */
    idx_t m = n + 1;          /* n+1 in unsigned arithmetic */
    idx_t r = 0;
    while (m > 1) {
        m >>= 1;
        ++r;
    }
    /* first index of this row: 2^r - 1 */
    idx_t first_of_row = ((idx_t)1 << r) - 1;
    idx_t k = n - first_of_row;   /* offset within row */

    /* obtain factorial of r */
    ensure_factorials(r);
    seq_t fact_r = fact_cache[r];

    /* unrank subset corresponding to this k */
    idx_t subset[64];           /* r <= 63 for all practical n (10k) */
    idx_t subset_len = 0;
    unrank_subset(r, k, subset, &subset_len);

    /* compute product of the complement of the subset in [1..r] */
    seq_t prod = (seq_t)1;
    idx_t prev = 1;
    for (idx_t i = 0; i < subset_len; ++i) {
        idx_t s = subset[i];
        /* multiply numbers from prev to s-1 */
        for (idx_t x = prev; x < s; ++x) {
            prod *= (seq_t)x;
        }
        prev = s + 1;
    }
    /* multiply remaining numbers after the last subset element */
    for (idx_t x = prev; x <= r; ++x) {
        prod *= (seq_t)x;
    }

    /* For the empty subset (k=0), prod equals r! as expected */
    return prod;
}
