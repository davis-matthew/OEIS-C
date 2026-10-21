/*
 * A065684  Number of primes <= prime(n) which begin with a 5.
 * https://oeis.org/A065684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A065684:  a(n) = number of primes ≤ prime(n) that start with the decimal
   digit 5.  The function generates the necessary primes on demand,
   keeps a running cumulative count of primes starting with 5, and
   returns the requested value.  All data are stored in static arrays,
   so calls can appear in arbitrary order. */

#include <stdlib.h>
#include <stddef.h>

/* forward declarations of helper functions */
static inline seq_t leading_digit(idx_t v);
static int is_prime_candidate(idx_t candidate, idx_t *prime_list, size_t prime_cnt);

/* ------------------------------------------------------------ */
/*  leading_digit - most significant decimal digit of v         */
static inline seq_t leading_digit(idx_t v)
{
    while (v >= 10) {
        v /= 10;
    }
    return (seq_t)v;
}

/* ------------------------------------------------------------ */
/*  is_prime_candidate - test whether *candidate* is prime
   using the primes already stored in prime_list[0 .. prime_cnt-1].  */
static int is_prime_candidate(idx_t candidate, idx_t *prime_list, size_t prime_cnt)
{
    /* trivial cases */
    if (candidate < 2) return 0;
    if (candidate == 2) return 1;
    if ((candidate & 1U) == 0) return 0;      /* even numbers > 2 are composite */

    /* trial division by known primes not exceeding sqrt(candidate) */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > candidate / p) break;         /* p*p > candidate */
        if (candidate % p == 0) return 0;
    }
    return 1;
}

/* ------------------------------------------------------------ */
/*  a - main function of the sequence                              */
seq_t a(idx_t n)
{
    /* static storage for primes and the cumulative counts */
    static idx_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_cnt = 0;

    static seq_t *prefix = NULL;
    static size_t prefix_cap = 0;
    static size_t prefix_cnt = 0;

    /* initial allocation for the first calls */
    if (primes_cap == 0) {
        primes_cap = 100;                     /* reasonable initial size */
        primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
        prefix_cap = primes_cap;
        prefix = (seq_t *)malloc(prefix_cap * sizeof(seq_t));
    }

    /* lazy initialisation of the first prime (2) */
    if (primes_cnt == 0) {
        primes[0] = 2;
        prefix[0] = 0;                        /* 2 does not start with 5 */
        primes_cnt = 1;
        prefix_cnt = 1;
    }

    /* generate primes until we have at least n of them */
    idx_t candidate = primes[primes_cnt - 1] + 1;     /* start searching */
    while (primes_cnt < n) {

        /* skip even candidates > 2 */
        if (candidate > 2 && (candidate & 1U) == 0) {
            ++candidate;
            continue;
        }

        if (is_prime_candidate(candidate, primes, primes_cnt)) {
            /* enlarge storage if necessary */
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
                prefix_cap *= 2;
                prefix = (seq_t *)realloc(prefix, prefix_cap * sizeof(seq_t));
            }

            /* store new prime */
            primes[primes_cnt] = candidate;

            /* cumulative count of primes starting with 5 */
            seq_t add = (leading_digit(candidate) == 5) ? 1 : 0;
            seq_t cnt = prefix[primes_cnt - 1] + add;
            prefix[primes_cnt] = cnt;

            ++primes_cnt;
            ++prefix_cnt;
        }
        ++candidate;
    }

    /* return the count for the first n primes */
    return prefix[n - 1];
}
