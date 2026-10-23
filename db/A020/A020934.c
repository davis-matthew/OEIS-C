/*
 * A020934  Greatest k such that (k-th prime) < (4 times n-th prime).
 * https://oeis.org/A020934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A020934
    a(n) = largest k such that prime(k) < 4 * prime(n)
    i.e. a(n) = π(4 * prime(n) - 1)   (π(x) = number of primes ≤ x)   */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- Prime storage --- */
static uint32_t *prime_list = NULL;   /* dynamic array of primes   */
static size_t prime_count = 0;        /* number of primes stored   */
static size_t prime_cap   = 0;        /* current capacity          */
static uint32_t last_checked = 1;     /* last integer that was checked for primality */

/* Append a prime to the list, expanding the array if necessary. */
static void add_prime(uint32_t p)
{
    if (prime_count == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 16;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
        if (!tmp) exit(1);                /* allocation failure */
        prime_list = tmp;
        prime_cap = new_cap;
    }
    prime_list[prime_count++] = p;
}

/* Trial‑division primality test using the list of already found primes. */
static bool is_prime_trial(uint32_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        uint64_t sq = (uint64_t)p * p;
        if (sq > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Make sure that at least `n` primes have been generated. */
static void ensure_primes(size_t n)
{
    while (prime_count < n) {
        ++last_checked;
        if (is_prime_trial(last_checked))
            add_prime(last_checked);
    }
}

/* Make sure primes are available up to `limit` (inclusive). */
static void ensure_primes_upto(uint32_t limit)
{
    for (++last_checked; last_checked <= limit; ++last_checked) {
        if (is_prime_trial(last_checked))
            add_prime(last_checked);
    }
}

/* Binary search: number of primes ≤ x.  Assumes prime_list is sorted. */
static size_t count_primes_le(uint32_t x)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;   /* count of primes ≤ x */
}

/* ------------------------------------------------------------ */
/*  Main function requested by the problem statement.           */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined offset – return 0 */

    size_t idx = (size_t)n;               /* safe conversion for loops */

    /* Ensure we have the first n primes. */
    ensure_primes(idx);

    /* p_n is the nth prime. */
    uint32_t p_n = prime_list[idx - 1];

    /* Bound is 4 * p_n; we need primes < bound. */
    uint64_t bound = 4ULL * p_n;

    /* Work with uint32_t as bound is well below 2^32 for all n of interest. */
    uint32_t bound32 = (uint32_t)bound;

    /* Make sure all primes up to bound-1 are available. */
    if (bound32 > 0) 
        ensure_primes_upto(bound32 - 1);

    /* Count primes ≤ bound-1, i.e. primes < bound. */
    size_t count = count_primes_le(bound32 - 1);

    return (seq_t)count;
}
