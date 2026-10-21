/*
 * A088445  a(n) = n / A088444(n), where A088444(n) is the smallest divisor d
 *          of n such that all intervals [(k-1)*d+1:k*d] contain at least one
 *          prime, 1<=k<=n/d; a(1)=1.
 * https://oeis.org/A088445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n / A088444(n), where A088444(n) is the smallest *
 *            divisor d of n such that every interval [(k-1)*d+1 : k*d] (1 ≤ k
 *            ≤ n/d) * contains at least one prime. The function is memoised
 *            with an * incremental sieve so that calls are fast for n up to
 *            ~10000
 * Range:     u8 n <= 500, u16 n <= 9597, u32 n <= 9597, u64 n <= 9597,
 *            u128 n <= 9597, big any n
 * Helpers:   divisors_sorted
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/divisors_sorted.h"

/* ----------  static prime tables  ---------- */
static bool   *prime_flags   = NULL;      /* is prime? up to current limit */
static idx_t prime_flags_cap = 0;         /* size of prime_flags array   */

static idx_t *prime_list   = NULL;       /* list of primes found so far */
static size_t prime_list_cap = 0;        /* capacity of prime_list      */
static size_t prime_list_cnt = 0;        /* number of primes            */

static idx_t *prime_prefix   = NULL;     /* prime count prefix array    */
static idx_t prime_prefix_cap = 0;       /* capacity of prime_prefix    */

/* ensure that primes up to `limit` are known and prime_prefix is built */
static void ensure_primes(idx_t limit)
{
    if (limit < prime_flags_cap) return;

    /* grow flags array */
    size_t new_cap = (size_t)limit + 1;
    if (prime_flags == NULL)
        prime_flags = (bool*)malloc(new_cap * sizeof(bool));
    else
        prime_flags = (bool*)realloc(prime_flags, new_cap * sizeof(bool));

    /* initialise new entries; 0 and 1 are not prime */
    for (idx_t i = prime_flags_cap; i <= limit; ++i) {
        if (i < 2) prime_flags[i] = false;
        else       prime_flags[i] = true;
    }

    /* sieve using the primes already known */
    idx_t sqrt_limit = (idx_t)(sqrt((double)limit));
    for (size_t pi = 0; pi < prime_list_cnt; ++pi) {
        idx_t p = prime_list[pi];
        if (p > sqrt_limit) break;
        idx_t start = p * p;
        if (start < prime_flags_cap)
            start = ((idx_t)(prime_flags_cap + p - 1) / p) * p;
        for (idx_t j = start; j <= limit; j += p)
            prime_flags[j] = false;
    }

    /* collect any new primes found */
    for (idx_t i = prime_flags_cap; i <= limit; ++i) {
        if (prime_flags[i]) {
            if (prime_list_cnt == prime_list_cap) {
                size_t new_cap_prime = prime_list_cap ? prime_list_cap * 2 : 64;
                prime_list = (idx_t*)realloc(prime_list, new_cap_prime * sizeof(idx_t));
                prime_list_cap = new_cap_prime;
            }
            prime_list[prime_list_cnt++] = i;
        }
    }
    prime_flags_cap = new_cap;

    /* build prefix array of prime counts */
    if (prime_prefix == NULL || prime_prefix_cap < new_cap) {
        if (prime_prefix) free(prime_prefix);
        prime_prefix   = (idx_t*)malloc(new_cap * sizeof(idx_t));
        prime_prefix_cap = new_cap;
    }
    prime_prefix[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        prime_prefix[i] = prime_prefix[i-1] + (prime_flags[i] ? 1 : 0);
}

/* number of primes in [L,R], assume 1 ≤ L ≤ R ≤ n */
static idx_t primes_in_interval(idx_t L, idx_t R)
{
    return prime_prefix[R] - prime_prefix[L-1];
}

/* ----------  public function  ---------- */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)n;   /* a(1)=1, a(0) undefined but safe */

    ensure_primes(n);

    idx_t divs[128];
    size_t dcnt;
    divisors_sorted(n, divs, &dcnt);

    for (size_t i = 0; i < dcnt; ++i) {
        idx_t d = divs[i];
        bool ok = true;
        idx_t blocks = n / d;
        for (idx_t k = 1; k <= blocks; ++k) {
            idx_t L = (k-1)*d + 1;
            idx_t R = k*d;
            if (primes_in_interval(L, R) == 0) {
                ok = false;
                break;
            }
        }
        if (ok) return (seq_t)n / (seq_t)d;
    }

    /* fall‑back (should never be reached) */
    return 1;
}
