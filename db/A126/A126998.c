/*
 * A126998  a(n) = binomial(prime(n+5), prime(5)).
 * https://oeis.org/A126998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(prime(n+5), 11)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 55, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 860 (2026-11-01)
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/gcd.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

static uint64_t *prime_array = NULL;
static size_t prime_count = 0;
static idx_t prime_max_index = 0;

/* Ensure that prime_array contains at least primes up to the given index
   (index is 1‑based). */
static void ensure_primes_up_to(idx_t target_index)
{
    if (prime_max_index >= target_index)
        return;

    /* Compute an upper bound for prime(target_index). */
    idx_t limit = _prime_limit_for_idx(target_index);

    /* Build all primes up to that limit. */
    uint64_t *new_arr = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_arr, &new_cnt);

    /* Store the array for future use. */
    prime_array   = new_arr;
    prime_count   = new_cnt;
    prime_max_index = target_index;   /* we are sure we have enough primes */
}

/* Compute binomial(n, k) where 0 <= k <= n and result fits in seq_t. */
static seq_t binomial_seq_t(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;   /* use symmetry */

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t numerator   = n - k + i;  /* (n-k+i) fits in idx_t */
        seq_t denominator = i;

        /* Reduce denominator with current result. */
        seq_t g = gcd(result, denominator);
        result   /= g;
        denominator /= g;

        /* Now reduce any remaining gcd between numerator and denominator. */
        g = gcd(numerator, denominator);
        numerator   /= g;
        denominator /= g;

        /* denominator should be 1 here; multiply result by numerator. */
        result *= numerator;
    }
    return result;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* prime index = n + 5 (1‑based) */
    idx_t prime_idx = n + 5;

    /* Make sure we have primes up to this index. */
    ensure_primes_up_to(prime_idx);

    /* prime(n+5) is stored at primes[prime_idx-1] of the array. */
    uint64_t p = prime_array[prime_idx - 1];

    /* prime(5) = 11 */
    return binomial_seq_t((idx_t)p, 11);
}
