/*
 * A037125  Irregular triangle: row n is 1, 2, 3, 4, .., prime(n).
 * https://oeis.org/A037125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   enlarge_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n − ∑_{k=1}^{r−1} p_k
 * where p_k is the k‑th prime and r is the smallest integer with
 * ∑_{k=1}^{r} p_k ≥ n.
 *
 * The sequence consists of the rows “1…p_k” concatenated for k = 1, 2, …
 * (offset 1).  The implementation keeps a dynamic array of primes and
 * the corresponding cumulative sums in static memory, expanding it as
 * needed.  This works in O(n) (linear) time for n up to ~10⁴ and is
 * easily fast enough for much larger n as well.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/enlarge_array.h"

/* static state ------------------------------------------------------------- */

static uint64_t *primes      = NULL;   /* primes[0] = 2, primes[1] = 3, ...      */
static seq_t   *cum        = NULL;   /* cum[i] = sum_{k=0}^{i} primes[k]      */
static size_t   prime_cnt  = 0;       /* number of primes stored              */
static size_t   prime_cap  = 0;       /* allocated capacity for primes        */
static size_t   cum_cnt    = 0;       /* number of cumulative sums stored     */
static size_t   cum_cap    = 0;       /* allocated capacity for cum          */

/* ------------------------------------------------------------------------- */
/* Utility: enlarge a dynamic array                                        */

/* ------------------------------------------------------------------------- */
/* Test whether x is prime using already known primes (trial division).      */

static bool is_prime_candidate(uint64_t x)
{
    if (x < 2)
        return false;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = primes[i];
        if (p > x / p)          /* p*p > x  -> stop */
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------------- */
/* Generate primes until the cumulative sum reaches at least 'limit'        */

static void ensure_cumulative(idx_t limit)
{
    /* nothing to do if already sufficient */
    if (cum_cnt > 0 && cum[cum_cnt - 1] >= limit)
        return;

    /* start searching from last known number */
    uint64_t candidate = (prime_cnt == 0) ? 2 : primes[prime_cnt - 1] + 1;

    while (true)
    {
        /* expand prime array if needed */
        enlarge_array(&primes, &prime_cnt, &prime_cap, &prime_cnt, sizeof(uint64_t));
        enlarge_array((uint64_t **)&cum, &cum_cnt, &cum_cap, &cum_cnt, sizeof(seq_t));

        /* find next prime */
        while (!is_prime_candidate(candidate))
            ++candidate;

        /* store it */
        primes[prime_cnt] = candidate;
        /* update cumulative sum */
        seq_t new_sum;
        if (cum_cnt == 0)
            new_sum = (seq_t)candidate;
        else
            new_sum = cum[cum_cnt - 1] + (seq_t)candidate;
        cum[cum_cnt] = new_sum;
        ++prime_cnt;
        ++cum_cnt;

        if (new_sum >= limit)
            break;
        ++candidate;
    }
}

/* ------------------------------------------------------------------------- */
/* Binary search: first index with cum[idx] >= n (cum is 0‑based).           */

static size_t find_row(idx_t n)
{
    size_t lo = 0, hi = cum_cnt - 1;
    while (lo < hi)
    {
        size_t mid = lo + (hi - lo) / 2;
        if (cum[mid] >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* ------------------------------------------------------------------------- */
/* Main sequence function --------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* undefined for n == 0, but harmless */

    /* make sure we have enough primes/cumulative sums for this n */
    ensure_cumulative(n);

    /* locate the row that contains position n */
    size_t row = find_row(n);

    /* sum before the current row (0 if first row) */
    seq_t prev_sum = (row == 0) ? (seq_t)0 : cum[row - 1];

    /* value at position n */
    return (seq_t)n - prev_sum;
}
