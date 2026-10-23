/*
 * A033557  2n - {largest prime < n}.
 * https://oeis.org/A033557
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - (largest prime < n)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A033557: 2n - {largest prime < n} */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for primes generated so far */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;
static idx_t primes_max = 0;

/* grow primes array */
static void _grow_primes(size_t needed)
{
    if (primes_cap < needed)
    {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        primes = tmp;
        primes_cap = new_cap;
    }
}

/* generate primes up to 'limit' inclusively, extending current list */
static void _ensure_primes_upto(idx_t limit)
{
    if (limit <= primes_max) return;

    if (!primes)
    {
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes_cnt = 0;
        primes_max = 0;
    }

    idx_t start = (primes_max < 2 ? 2 : primes_max + 1);
    for (idx_t cand = start; cand <= limit; ++cand)
    {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i)
        {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0)
            {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
        {
            _grow_primes(primes_cnt + 1);
            primes[primes_cnt++] = cand;
        }
    }
    primes_max = limit;
}

/* return the largest prime < n (n >= 3) */
static uint64_t _largest_prime_lt(idx_t n)
{
    if (n <= 3) return 2;                 /* 2 is the only prime < 3 */
    _ensure_primes_upto(n - 1);           /* primes up to n-1 */

    /* binary search for the first prime >= n */
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi)
    {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < n)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo is first index with primes[lo] >= n, so largest < n is lo-1 */
    if (lo == 0) return 2;  /* safety, should not happen for n>=3 */
    return primes[lo - 1];
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;  /* undefined for n < 3, but harmless */

    uint64_t p = _largest_prime_lt(n);
    seq_t res = (seq_t)n;
    res <<= 1;          /* res = 2*n */
    res -= (seq_t)p;    /* res = 2*n - p */
    return res;
}
