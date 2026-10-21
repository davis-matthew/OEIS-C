/*
 * A066888  Number of primes p between triangular numbers T(n) < p <= T(n+1).
 * https://oeis.org/A066888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = pi( T(n+1) ) - pi( T(n) ), where T(k) = k*(k+1)/2 is the
 *            k‑th triangular number
 * Range:     u8 n <= 21, u16 n <= 900, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sieve_primes.h"

/* static state to cache primes up to the largest triangular number seen */
static idx_t *cached_primes = NULL;      /* array of primes */
static size_t  cached_cnt   = 0;          /* number of primes stored   */
static idx_t   cached_limit = 0;          /* highest value sieved up to */

/* simple binary search returning count of primes <= x */
static size_t prime_leq_count(idx_t x)
{
    size_t lo = 0, hi = cached_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (cached_primes[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* ensure we have sieved at least up to 'limit' */
static void ensure_sieve(idx_t limit)
{
    if (limit <= cached_limit)
        return;                       /* already sufficient */

    /* release old array if it exists */
    if (cached_primes) {
        free(cached_primes);
        cached_primes = NULL;
        cached_cnt   = 0;
    }
    /* build sieve up to 'limit' */
    sieve_primes(limit, &cached_primes, &cached_cnt);
    cached_limit = limit;
}

seq_t a(idx_t n)
{
    /* compute triangular numbers as seq_t */
    seq_t n_seq   = (seq_t)n;
    seq_t Tn      = n_seq * (n_seq + 1) / 2;
    seq_t Tn_plus = (n_seq + 1) * (n_seq + 2) / 2;

    /* ensure we have primes up to T(n+1) */
    ensure_sieve((idx_t)Tn_plus);

    /* count primes <= each triangular number using binary search */
    size_t pi_Tn   = prime_leq_count((idx_t)Tn);
    size_t pi_Tn1  = prime_leq_count((idx_t)Tn_plus);

    /* difference gives the required count */
    return (seq_t)(pi_Tn1 - pi_Tn);
}
