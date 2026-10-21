/*
 * A200927  Difference between (least semiprime >= n) and (largest semiprime
 *          <= n).
 * https://oeis.org/A200927
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least semiprime ≥ n – greatest semiprime ≤ n (difference
 *            of closest semiprimes around n, a(n) = 0 iff n is semiprime)
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* utilities -------------------------------------------------------------- */
#include "../utils/floor_sqrt.h"        /* seq_t floor_sqrt(seq_t)            */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit,
                                         uint64_t **primes_out, size_t *cnt_out) */

/* static data ------------------------------------------------------------- */
static uint64_t *primes = NULL;     /* list of primes           */
static size_t    primes_cnt = 0;    /* count of primes          */
static idx_t     prime_limit_max = 0; /* maximum limit for primes */

/* Ensure that primes up to 'limit' are available. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit_max) return;
    /* build primes up to 'limit' and store them */
    uint64_t *p = NULL;
    size_t   c = 0;
    build_primes_upto(limit, &p, &c);
    /* free old list if present */
    free(primes);
    primes = p;
    primes_cnt = c;
    prime_limit_max = limit;
}

/* test whether x is a semiprime (product of exactly two primes,
   counting multiplicity). */
static bool is_semiprime(idx_t x)
{
    if (x < 4) return false;          /* smallest semiprime is 4 */

    /* need primes up to sqrt(x) */
    ensure_primes_upto((idx_t)floor_sqrt((seq_t)x));

    int count = 0;
    idx_t tmp = x;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if ((idx_t)p * (idx_t)p > tmp) break;
        while (tmp % p == 0) {
            ++count;
            if (count > 2) return false;
            tmp /= p;
        }
        if (tmp == 1) break;
    }
    if (tmp > 1) ++count;             /* remaining prime factor */

    return count == 2;
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* find largest semiprime <= n */
    idx_t largest = 0;
    for (idx_t m = n; ; --m) {
        if (m < 4) break;
        if (is_semiprime(m)) { largest = m; break; }
    }

    /* find smallest semiprime >= n */
    idx_t smallest = n;
    while (!is_semiprime(smallest)) ++smallest;

    return (seq_t)(smallest - largest);   /* non‑negative */
}
