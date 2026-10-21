/*
 * A088239  Triangle read by rows: T(n,k) is the number of primes not less
 *          than n-k and not greater than n+k, 0<=k<n.
 * https://oeis.org/A088239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of primes p with r-k ≤ p ≤ r+k, where n is the nth
 *            entry of the triangle read by rows and r is the row (1‑based)
 *            such that r(r+1)/2 ≥ n. k = n - (r-1)r/2 - 1 (0‑based column)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static state for the prime sieve */
static seq_t *prime_flag = NULL;   /* one byte per number, 1=prime, 0=not */
static idx_t  prime_limit = 0;     /* maximum number sieved so far */

/* Ensure the sieve covers all integers up to 'lim' (inclusive). */
static void ensure_prime_limit(idx_t lim)
{
    if (prime_limit >= lim)
        return;  /* already sufficient */

    /* Free old flag array if any */
    free(prime_flag);

    /* Allocate new array, index 0..lim */
    prime_flag = (seq_t *)malloc((size_t)(lim + 1) * sizeof(seq_t));
    if (!prime_flag)
        exit(EXIT_FAILURE);

    /* Initialize all as prime (1). Numbers 0 and 1 are not prime. */
    for (idx_t i = 0; i <= lim; ++i)
        prime_flag[i] = 1;
    if (lim >= 0) prime_flag[0] = 0;
    if (lim >= 1) prime_flag[1] = 0;

    /* Classic sieve of Eratosthenes */
    for (idx_t i = 2; (idx_t)(i * i) <= lim; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= lim; j += i)
                prime_flag[j] = 0;
        }
    }

    prime_limit = lim;
}

/* Count primes in the inclusive interval [low, high].  low ≤ high. */
static seq_t count_primes_in_range(idx_t low, idx_t high)
{
    seq_t cnt = 0;
    if (low < 2) low = 2;
    for (idx_t i = low; i <= high; ++i) {
        if (prime_flag[i])
            ++cnt;
    }
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1, but guard for safety */
        return 0;

    /* Find the smallest r such that r*(r+1)/2 >= n */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 prod = (__int128)mid * (mid + 1) / 2;
        if (prod >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;  /* 1‑based row index */

    /* Total entries before current row */
    __int128 prev_total = (__int128)(r - 1) * r / 2;
    idx_t prev_total_ui = (idx_t)prev_total;

    /* Column index k (0‑based) */
    idx_t k = n - prev_total_ui - 1;  /* 0 ≤ k < r */

    /* Bounds of the prime interval */
    idx_t low_limit  = r - k;
    idx_t high_limit = r + k;

    /* Ensure sieve covers up to high_limit. */
    ensure_prime_limit(high_limit);

    /* Count primes in [low_limit, high_limit] */
    return count_primes_in_range(low_limit, high_limit);
}
