/*
 * A134807  Partition numbers of Mersenne primes.
 * https://oeis.org/A134807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(A000668(n))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime, is_prime_big, seq_alloc, seq_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
/* helper functions that are provided by the build system */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_pow.h"
#include "../utils/is_prime_big.h"
#include "../utils/is_prime.h"

/* Cache for Mersenne primes */
static seq_t *mers_cache = NULL;
static idx_t   mers_count = 0;
static idx_t   mers_max   = 0;

/* Cache for partition numbers */
static seq_t *part_cache = NULL;
static idx_t   part_max   = 0;

/* Compute nth Mersenne prime (value as seq_t) */
static seq_t nth_mersenne(idx_t n)
{
    /* Expand cache if needed */
    while (mers_count < n)
    {
        idx_t exp = mers_max + 1;
        /* Skip even exponents (except 2) */
        if (exp > 2 && (exp & 1) == 0) { exp++; }
        /* Find next prime exponent */
        while (!is_prime(exp)) exp++;
        /* Compute 2^exp - 1 */
        seq_t mers = seq_pow((seq_t)2, exp) - (seq_t)1;
        if (is_prime_big(mers))
        {
            /* Store in cache */
            size_t new_cap = mers_count + 1;
            if (!mers_cache)
                mers_cache = seq_alloc(new_cap);
            else
                mers_cache = seq_realloc(mers_cache, mers_count, new_cap);
            mers_cache[mers_count] = mers;
            mers_count++;
            mers_max = exp;
        }
        mers_max = exp;
    }
    return mers_cache[n-1];
}

/* Ensure that partition numbers up to m are computed */
static void ensure_partitions(idx_t m)
{
    if (part_max >= m) return;
    size_t new_cap = m + 1;
    if (!part_cache)
        part_cache = seq_alloc(new_cap);
    else
        part_cache = seq_realloc(part_cache, part_max + 1, new_cap);

    /* Initialization if starting from scratch */
    if (part_max == 0)
    {
        part_cache[0] = (seq_t)1;
        part_max = 0;
    }

    /* Compute partitions from part_max+1 to m */
    for (idx_t n = part_max + 1; n <= m; ++n)
    {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;
        for (idx_t k = 1; ; ++k)
        {
            idx_t pent1 = k * (3 * k - 1) / 2;
            idx_t pent2 = k * (3 * k + 1) / 2;
            bool added = false;
            bool neg = (k % 2 == 0);
            if (n >= pent1)
            {
                if (neg) sum_neg += part_cache[n - pent1];
                else     sum_pos += part_cache[n - pent1];
                added = true;
            }
            if (n >= pent2)
            {
                if (neg) sum_neg += part_cache[n - pent2];
                else     sum_pos += part_cache[n - pent2];
                added = true;
            }
            if (!added) break;
        }
        part_cache[n] = sum_pos - sum_neg;
    }
    part_max = m;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, returns 0 */
    seq_t mers = nth_mersenne(n);
    /* mers is the n-th Mersenne prime */
    idx_t m = (idx_t)mers;
    ensure_partitions(m);
    return part_cache[m];
}
