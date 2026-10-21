/*
 * A062823  Numbers k such that the number of divisors of k divides the number
 *          of divisors of the totient of k.
 * https://oeis.org/A062823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Search for the next integer k such that the number of divisors
 *            of k divides the number of divisors of φ(k). The function
 *            returns the n-th term of OEIS A062823 (offset 1). The sequence
 *            is stored as a strictly increasing list of filtered integers
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Forward declarations of the helper functions that will be defined
   as static inline below. */
static void ensure_primes(seq_t limit);
static int satisfies(seq_t k);

/* --------------------------------------------------------------------- */
/* Global static data used to memoise the computed terms and the prime
   list.  These are shared across calls to `a`. */
static idx_t found_cnt = 0;          /* number of terms already stored   */
static seq_t *found = NULL;          /* dynamic array of terms           */
static size_t found_cap = 0;         /* capacity of the array            */

static seq_t curr = 1;               /* last candidate examined         */

static seq_t *primes = NULL;         /* dynamic array of primes         */
static size_t primes_cap = 0;        /* capacity of primes array        */
static size_t primes_cnt = 0;        /* number of primes stored         */
static seq_t primes_limit = 0;       /* highest number checked for prime*/

static void ensure_primes(seq_t limit) {
    if (limit <= primes_limit)
        return;

    /* First generate all primes up to the new limit.  We generate
       only odd candidates after 2.  This is a simple trial‑division
       sieve that is fast enough for the ranges we need. */
    seq_t start = primes_limit < 2 ? 2 : primes_limit + 1;
    if (start <= 2) {
        /* add 2 if not already present */
        if (primes_cnt == 0) {
            if (primes_cap == 0) {
                primes_cap = 16;
                primes = (seq_t *)malloc(primes_cap * sizeof(seq_t));
            }
            primes[primes_cnt++] = 2;
        }
        start = 3;
    }

    for (seq_t n = start; n <= limit; n += 2) {
        int is_prime = 1;
        for (size_t i = 0; i < primes_cnt; ++i) {
            seq_t p = primes[i];
            if (p * p > n)
                break;
            if (n % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (seq_t *)realloc(primes, primes_cap * sizeof(seq_t));
            }
            primes[primes_cnt++] = n;
        }
    }
    primes_limit = limit;
}

static int satisfies(seq_t k) {
    /* Ensure we have enough primes to factor k and φ(k). */
    ensure_primes(k);

    /* Compute d(k) and φ(k). */
    seq_t n = k;
    seq_t d_k = 1;
    seq_t phi = k;

    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= n; ++i) {
        seq_t p = primes[i];
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            d_k *= (exp + 1);
            phi = phi / p * (p - 1);
        }
    }
    if (n > 1) {  /* remaining prime factor */
        d_k *= 2;
        phi = phi / n * (n - 1);
    }

    /* Compute d(φ(k)). */
    seq_t m = phi;
    seq_t d_phi = 1;
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= m; ++i) {
        seq_t p = primes[i];
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            d_phi *= (exp + 1);
        }
    }
    if (m > 1) {
        d_phi *= 2;
    }

    return (d_phi % d_k == 0);
}

seq_t a(idx_t n) {
    /* Initialise the memoised list on first call. */
    if (!found) {
        found_cap = 64;
        found = (seq_t *)malloc(found_cap * sizeof(seq_t));
        /* The first term is k = 1. */
        found[0] = (seq_t)1;
        found_cnt = 1;
        curr = 1;
    }

    while (found_cnt < n) {
        /* Find next candidate that satisfies the condition. */
        do {
            ++curr;
        } while (!satisfies(curr));

        /* Store the found term. */
        if (found_cnt == found_cap) {
            found_cap *= 2;
            found = (seq_t *)realloc(found, found_cap * sizeof(seq_t));
        }
        found[found_cnt++] = curr;
    }

    return found[n - 1];
}
