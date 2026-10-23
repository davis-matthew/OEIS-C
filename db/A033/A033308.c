/*
 * A033308  Decimal expansion of Copeland-Erdős constant: concatenate primes.
 * https://oeis.org/A033308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, base, changed
 * Formula:   not stated
 * Range:     u8 n <= 197, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   build_primes, digit_count, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A033308 – digit n of the decimal expansion of
 *  the concatenation of the prime numbers 2,3,5,7,11,13,17,...
 *  a(0)=2, a(1)=3, a(2)=5, a(3)=7, a(4)=1, a(5)=1, a(6)=1, …
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_count.h"
#include "../utils/get_digits.h"
#include "../utils/build_primes.h"

/* static cache of primes and cumulative digit counts */
static idx_t cached_limit = 0;
static uint64_t *cached_primes = NULL;
static size_t cached_prime_cnt = 0;
static idx_t *cum_digits = NULL;   /* cumulative digit counts            */
static size_t cum_cnt = 0;
static int initialized = 0;

/* rebuild the cached data up to new_limit primes */
static void rebuild(idx_t new_limit)
{
    size_t new_cnt = 0;
    uint64_t *new_prime_arr = NULL;

    /* build a prime list up to new_limit */
    build_primes(new_limit, (idx_t **)&new_prime_arr, &new_cnt);

    /* free previous cache */
    free(cached_primes);
    free(cum_digits);

    cached_primes   = new_prime_arr;
    cached_prime_cnt = new_cnt;
    cached_limit    = new_limit;

    /* build cumulative digit array */
    cum_digits = (idx_t *)malloc(new_cnt * sizeof(idx_t));
    idx_t acc = 0;
    for (size_t i = 0; i < new_cnt; ++i)
    {
        acc += digit_count((uint64_t)cached_primes[i]);
        cum_digits[i] = acc;
    }
    cum_cnt = new_cnt;
}

/* ensure that cum_digits holds a value > need */
static void ensure_cumulative(idx_t need)
{
    if (!initialized)
    {
        /* initialise with a modest limit */
        cached_limit = 1000;
        rebuild(cached_limit);
        initialized = 1;
    }
    /* extend until the last cumulative digit count exceeds need */
    while (cum_cnt && cum_digits[cum_cnt - 1] <= need)
    {
        cached_limit *= 2;
        rebuild(cached_limit);
    }
}

/* binary search for first index i with cum_digits[i] > need */
static size_t find_prime_index(idx_t need)
{
    size_t lo = 0, hi = cum_cnt;
    while (lo < hi)
    {
        size_t mid = (lo + hi) / 2;
        if (cum_digits[mid] > (idx_t)need)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* the sequence function */
seq_t a(idx_t n)
{
    ensure_cumulative(n);

    size_t pi = find_prime_index(n);
    if (pi == cum_cnt)      /* safety: n is beyond cached range */
        return 0;

    idx_t prev_cum = (pi == 0) ? 0 : cum_digits[pi - 1];
    idx_t offset   = n - prev_cum;   /* 0‑based offset within prime */

    /* retrieve digits of the prime */
    uint8_t buf[20];          /* 64‑bit prime fits in at most 20 decimal digits */
    size_t len = 0;
    get_digits((seq_t)cached_primes[pi], buf, &len);

    /* get_digits returns digits with least‑significant first,
       so we must reverse the index */
    return (seq_t)buf[len - 1 - offset];
}
