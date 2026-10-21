/*
 * A087011  Number of primes of form 4*k+3 between n and 2n (inclusive).
 * https://oeis.org/A087011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p with n ≤ p ≤ 2n and p ≡ 3 (mod 4)
 * Range:     u8 n <= 4505, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Prime sieve cache ----------------------------------------------------- */
static bool *prime_flag = NULL;   /* prime_flag[i] == false  -> i is prime  (2 <= i <= limit) */
static idx_t sieve_limit = 0;     /* highest index for which prime_flag is valid */

/* Rebuild the sieve up to (and including) 'limit' */
static void
ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;
    /* free old cache if any */
    free(prime_flag);

    /* Allocate new flag array, initialized to false (prime) */
    prime_flag = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!prime_flag) abort();          /* out of memory */

    /* Mark 0 and 1 as composite */
    if (limit >= 0) prime_flag[0] = true;
    if (limit >= 1) prime_flag[1] = true;

    /* Sieve of Eratosthenes */
    idx_t sqrt_limit = (idx_t)floor(sqrt((double)limit));
    for (idx_t i = 2; i <= sqrt_limit; ++i) {
        if (!prime_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_flag[j] = true;
        }
    }

    sieve_limit = limit;
}

seq_t
a(idx_t n)
{
    /* Compute the range [n, 2n] */
    idx_t high = n * 2;          /* we assume n is not so large that this overflows */
    if (high < n) high = UINT64_MAX;   /* fallback for overflow (unlikely here) */

    /* Ensure sieve covers the upper bound */
    ensure_sieve(high);

    /* Count primes of the form 4k+3 in the interval */
    seq_t count = 0;
    for (idx_t p = n; p <= high; ++p) {
        if (!prime_flag[p] && (p & 3) == 3) {
            ++count;
        }
    }
    return count;
}
