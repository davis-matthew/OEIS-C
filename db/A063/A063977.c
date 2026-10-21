/*
 * A063977  Numbers which are sums of unitary divisors, the usigma values:
 *          their inverse usigma set is not empty; usigma() = A034448().
 * https://oeis.org/A063977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest value of the unitary sum σᵤ(m) where
 *            σᵤ(m) = ∏_{p^e‖m} (1 + p^e)
 * Range:     u8 none, u16 n <= 30, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation precomputes σᵤ(m) for m up to a running limit,
    stores all distinct values in sorted order, and returns the
    n-th element.  The table is grown automatically if the requested
    index exceeds the precomputed size.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

/* Comparison function for qsort on uint128_t or other seq_t types */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Helper to compute unitary sum for a single integer m > 0.  */
static seq_t usigma_of(idx_t m, uint32_t *spf)
{
    seq_t prod = (seq_t)1;
    idx_t x = m;
    while (x > 1) {
        uint32_t p = spf[x];
        idx_t e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        seq_t term = big_pow((seq_t)p, (seq_t)e);
        prod = prod * (term + (seq_t)1);
    }
    return prod;
}

/* Recompute the table up to the given limit.  Returns the new size
   of the distinct list.  */
static size_t recompute(idx_t limit,
                        seq_t **out_order, size_t *out_size)
{
    /* Allocate spf array */
    uint32_t *spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    if (!spf) abort();

    /* Sieve for smallest prime factor */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i > limit / i) continue;
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += i) {
                if (spf[j] == 0) spf[(size_t)j] = (uint32_t)i;
            }
        }
    }

    /* Compute σᵤ(m) for all m <= limit */
    seq_t *raw = (seq_t *)malloc((size_t)limit * sizeof(seq_t));
    if (!raw) abort();
    for (idx_t m = 1; m <= limit; ++m) {
        raw[(size_t)m - 1] = usigma_of(m, spf);
    }

    free(spf);

    /* Sort raw values */
    qsort(raw, (size_t)limit, sizeof(seq_t), cmp_seq);

    /* Deduplicate */
    size_t uniq = 0;
    seq_t *order = NULL;
    if (limit > 0) {
        order = (seq_t *)malloc((size_t)limit * sizeof(seq_t));
        if (!order) abort();
    }
    for (size_t i = 0; i < (size_t)limit; ++i) {
        if (i == 0 || raw[i] != raw[i - 1]) {
            order[uniq++] = raw[i];
        }
    }

    free(raw);

    *out_order = order;
    *out_size = uniq;
    return uniq;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    static idx_t curr_limit = 0;
    static seq_t *order = NULL;
    static size_t order_size = 0;
    if (n == 0) return (seq_t)0;  /* Offset is 1, but protect against 0 */

    /* Ensure table initialized and large enough for requested n */
    if (order == NULL) {
        curr_limit = 50000;
        order_size = recompute(curr_limit, &order, &order_size);
    }
    /* Grow table if needed */
    while ((idx_t)order_size < n) {
        curr_limit *= 2;
        free(order);
        order_size = recompute(curr_limit, &order, &order_size);
    }

    return order[n - 1];
}
