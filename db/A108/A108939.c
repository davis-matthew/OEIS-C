/*
 * A108939  Triangle read by rows in which row n lists all primes p such that
 *          p-1|n.
 * https://oeis.org/A108939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 684, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n‑th prime in the triangle read by rows,
   where row r lists all primes p such that p-1 divides r.
   The function generates required terms on demand using a sieve
   of primes up to a fixed limit and accumulates the row lengths. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/simple_sieve.h"

static uint32_t *primes = NULL;
static size_t primes_count = 0;
static idx_t *cum = NULL;
static size_t cum_size = 0;
static size_t max_precomputed_rows = 0;

/* Ensure primes and cumulative arrays are initialized. */
static void init_sequence(void)
{
    if (primes != NULL)
        return; /* already initialized */

    /* Large enough limit for all n up to 10000. */
    const uint32_t primes_limit = 200000;
    uint32_t *primes_out = NULL;
    size_t count_out = 0;
    simple_sieve(primes_limit, &primes_out, &count_out);
    primes = primes_out;
    primes_count = count_out;

    /* Allocate cumulative array for rows up to primes_limit+1. */
    cum_size = primes_limit + 2; /* rows indexed from 0..primes_limit+1 */
    cum = (idx_t *)malloc(cum_size * sizeof(idx_t));
    cum[0] = 0;
    max_precomputed_rows = 0;
}

/* Compute and extend cumulative row lengths until cum[r] >= n. */
static void extend_cum(idx_t n)
{
    while (cum[max_precomputed_rows] < n)
    {
        size_t r = max_precomputed_rows + 1; /* next row index (1-based) */
        size_t len = 0;
        /* Count primes p <= r+1 with (r % (p-1) == 0). */
        for (size_t i = 0; i < primes_count && primes[i] <= r + 1; ++i)
            if (r % (primes[i] - 1) == 0)
                ++len;
        cum[r] = cum[r - 1] + (idx_t)len;
        ++max_precomputed_rows;
    }
}

/* Binary search to find row r such that cum[r-1] < n <= cum[r]. */
static size_t find_row(idx_t n)
{
    size_t low = 1, high = max_precomputed_rows;
    while (low < high)
    {
        size_t mid = (low + high) >> 1;
        if (cum[mid] >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined, but return 0 for safety */

    init_sequence();
    extend_cum(n);

    size_t r = find_row(n);
    idx_t offset = n - cum[r - 1] - 1; /* zero‑based index within the row */

    /* Find the offset‑th prime satisfying p-1 | r. */
    for (size_t i = 0; i < primes_count && primes[i] <= r + 1; ++i)
    {
        if (r % (primes[i] - 1) == 0)
        {
            if (offset == 0)
                return (seq_t)primes[i];
            --offset;
        }
    }

    /* Should never reach here if logic is correct. */
    return (seq_t)0;
}
