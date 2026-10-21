/*
 * A089191  Primes p such that p+1 is cubefree.
 * https://oeis.org/A089191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 4558, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the n‑th prime p such that p+1 is cubefree.
 * The implementation builds primes incrementally and caches the
 * qualifying primes.  All arithmetic is done using the integer
 * types supplied by the build (seq_t, idx_t).                 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Global state                                                    */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list   = NULL;   /* primes discovered so far      */
static size_t   prime_cnt    = 0;        /* number of primes stored       */
static size_t   prime_cap    = 0;        /* allocated capacity for primes */

static seq_t   *qual_list    = NULL;   /* qualifying primes (cube‑free) */
static size_t   qual_cnt     = 0;        /* number of qualifying primes   */
static size_t   qual_cap     = 0;        /* allocated capacity            */

static uint64_t sieve_limit   = 0;       /* largest number up to which primes were generated */

/* ------------------------------------------------------------------ */
/*  Utility functions                                                */
/* ------------------------------------------------------------------ */

/* Append a prime to the prime list, expanding the array if necessary */
static void add_prime_to_list(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 8;
        uint64_t *tmp = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
        prime_list = tmp;
    }
    prime_list[prime_cnt++] = p;
}

/* Append a qualifying prime to the qual list, expanding if necessary */
static void add_qual_to_list(seq_t p)
{
    if (qual_cnt == qual_cap) {
        qual_cap = qual_cap ? qual_cap * 2 : 8;
        seq_t *tmp = (seq_t*)realloc(qual_list, qual_cap * sizeof(seq_t));
        qual_list = tmp;
    }
    qual_list[qual_cnt++] = p;
}

/* Test if a number n is prime using the primes already known */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Test if x is cubefree (does not contain a factor p³) */
static bool is_cubefree(uint64_t x)
{
    uint64_t tmp = x;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > tmp) break;
        unsigned int cnt = 0;
        while (tmp % p == 0) {
            ++cnt;
            tmp /= p;
            if (cnt >= 3) return false;
        }
    }
    /* any remaining factor > sqrt(tmp) has exponent 1 */
    return true;
}

/* Generate primes up to (and including) `limit`.  New primes are
 * added to the global prime list, and each new prime is checked
 * for the cubefree condition on p+1.  All qualifying primes are
 * appended to the global qual list. */
static void generate_primes_up_to(uint64_t limit)
{
    for (uint64_t cand = sieve_limit + 1; cand <= limit; ++cand) {
        if (is_prime_uint64(cand)) {
            add_prime_to_list(cand);
            if (cand > 1) {                     /* 2+1=3 is trivially cube‑free */
                if (is_cubefree(cand + 1))
                    add_qual_to_list((seq_t)cand);
            }
        }
    }
    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                         */
/* ------------------------------------------------------------------ */

/*
 * a(n) – the n‑th prime p such that p+1 is cubefree.
 * (Sequence starts at a(1)=2.)
 * The function lazily builds a table of qualifying primes and returns
 * the requested element.  Calls may come in any order; the internal
 * cache guarantees correct results for all n ≥ 1.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of range request */

    /* Keep generating primes until we have at least n qualifying primes */
    while ((idx_t)qual_cnt < n) {
        uint64_t next_limit = (sieve_limit < 10) ? 10 : sieve_limit * 2;
        generate_primes_up_to(next_limit);
    }

    return qual_list[n - 1];        /* array is 0‑based, sequence is 1‑based */
}
