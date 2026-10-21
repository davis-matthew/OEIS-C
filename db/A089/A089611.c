/*
 * A089611  Number of primes between n^2 and (n+1/3)^2.
 * https://oeis.org/A089611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = π( floor((n+1/3)^2) ) – π(n^2) (number of primes p with
 *            n^2 < p ≤ floor((n+1/3)^2))
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   sieve_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/sieve_upto.h"

/* binary search: returns the first index i with prime_list[i] > val.
   If all primes are ≤ val, returns prime_cnt. */
static size_t upper_bound(const idx_t *prime_list, size_t prime_cnt, idx_t val)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] <= val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* static cache of primes */
static idx_t *prime_list = NULL;
static size_t prime_cnt  = 0;
static idx_t sieve_limit = 0;

/* ----------------- main sequence function ----------------- */
seq_t a(idx_t n)
{
    /* compute lower = n^2, upper = n^2 + floor((2*n)/3) */
    unsigned __int128 nn = (unsigned __int128)n * n;
    idx_t lower  = (idx_t)nn;
    idx_t upper  = (idx_t)(nn + (unsigned __int128)(2 * n) / 3);

    if (upper > sieve_limit) {
        /* rebuild sieve up to new upper bound */
        if (prime_list) { free(prime_list); prime_list = NULL; }
        sieve_upto(upper, &prime_list, &prime_cnt);
        sieve_limit = upper;
    }

    /* Count primes ≤ upper and ≤ lower */
    size_t cnt_le_upper = upper_bound(prime_list, prime_cnt, upper);
    size_t cnt_le_lower = upper_bound(prime_list, prime_cnt, lower);

    return (seq_t)(cnt_le_upper - cnt_le_lower);
}
