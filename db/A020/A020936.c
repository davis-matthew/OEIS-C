/*
 * A020936  Greatest k such that (k-th prime) < (6 times n-th prime).
 * https://oeis.org/A020936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A020936: a(n) = greatest k such that p_k < 6 * p_n,
 * where p_n is the n‑th prime (1‑based). */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Static state: prime list and its bounds */
static uint64_t *prime_list = NULL;      /* array of primes, ascending */
static size_t    prime_cnt   = 0;        /* number of primes cached */
static idx_t     prime_max   = 0;        /* largest prime value cached */

/* Ensure primes are known up to the inclusive limit `limit` (if not already). */
static void
ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_max)
        return;                                /* already sufficient */

    /* Simple sieve of Eratosthenes up to `limit`. */
    size_t sz = (size_t)(limit + 1);           /* array size, max = 2^64 -> overflow is OK */
    bool *sieve = (bool *)malloc(sz);
    if (!sieve)
        exit(1);                               /* allocation failure */
    memset(sieve, true, sz);
    if (sz > 0) sieve[0] = false;
    if (sz > 1) sieve[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (sieve[i])
            for (idx_t j = i * i; j <= limit; j += i)
                sieve[j] = false;

    /* Count primes to allocate array. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i])
            ++cnt;

    /* Build the prime array. */
    uint64_t *newlist = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!newlist)
        exit(1);
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve[i])
            newlist[idx++] = i;

    free(sieve);

    /* Replace old cache. */
    free(prime_list);
    prime_list = newlist;
    prime_cnt   = cnt;
    prime_max   = limit;
}

/* Binary search: return the number of primes strictly less than `upper`. */
static seq_t
max_k_lt(seq_t upper)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if ((seq_t)prime_list[mid] < upper)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (seq_t)lo;         /* lo is the desired k */
}

/* Public sequence function. */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;             /* undefined for n<1 */

    /* Ensure we can look up the n-th prime. */
    if (prime_cnt < (size_t)n) {
        idx_t limit = 15;             /* initial bound */
        while (prime_cnt < (size_t)n) {
            if (limit < 32) limit = 31;
            else            limit = limit * 2 + 100; /* grow generously */
            ensure_primes_upto(limit);
        }
    }

    /* Select the n-th prime, p_n. */
    uint64_t pn = prime_list[n - 1];

    /* Compute upper bound: 6 * p_n. */
    seq_t upper = (seq_t)pn * (seq_t)6;

    /* Ensure all primes < upper are cached. */
    /* Cast upper to idx_t when it is safe. If it overflows, the sieve
       would be too large; in the problem limits this does not occur. */
    if (upper > (seq_t)UINT64_MAX)
        return max_k_lt(upper);   /* if overflow, count via comparison only */

    idx_t limit2 = (idx_t)upper;
    if (limit2 > prime_max)
        ensure_primes_upto(limit2);

    /* Binary search for the largest k with p_k < upper. */
    return max_k_lt(upper);
}
