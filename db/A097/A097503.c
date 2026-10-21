/*
 * A097503  Numbers k such that A000203(k) = sigma(k) is not divisible by 6.
 * https://oeis.org/A097503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that sigma(k) % 6 != 0
 *            (sigma(k) is the sum of the positive divisors of k)
 * Range:     u8 n <= 44, u16 n <= 4103, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  All auxiliary data structures are static so that subsequent calls
    can use previously computed values.  The implementation uses a
    linear sieve to compute the smallest prime factor (spf) for every
    integer in the current range, which allows fast factorisation of
    each n, and hence the divisor sum sigma(n).  The sieve is enlarged
    only when more numbers need to be processed.  Each term that
    satisfies the condition is stored in a dynamic array `terms`.  */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* static state */
static idx_t spf_limit = 0;          /* current size of spf array      */
static uint64_t *spf = NULL;         /* spf[0..spf_limit]              */
static size_t prime_cap = 0;         /* capacity of primes array       */
static size_t prime_cnt = 0;         /* number of primes found         */
static uint64_t *primes = NULL;      /* primes list                    */
static size_t terms_cap = 0;         /* terms array capacity           */
static size_t terms_len = 0;         /* number of terms stored         */
static seq_t *terms = NULL;          /* terms array                    */
static idx_t scanned = 0;            /* last integer that has been
                                       processed for sigma          */

/* Ensure that the spf array is enlarged to at least 'limit' */
static void ensure_spf(idx_t limit) {
    if (limit <= spf_limit) return;

    /* allocate new size, at most twice the current one to avoid many
       reallocations */
    idx_t new_limit = limit;
    if (new_limit < 2) new_limit = 2;
    if (new_limit < spf_limit * 2) new_limit = spf_limit * 2;

    uint64_t *new_spf = (uint64_t *)realloc(spf, (new_limit + 1) * sizeof(uint64_t));
    if (!new_spf) abort();
    spf = new_spf;

    /* initialise new entries to 0 (unknown spf) */
    for (idx_t i = spf_limit + 1; i <= new_limit; ++i) spf[i] = 0;
    if (spf_limit == 0) spf[0] = 0, spf[1] = 1;

    /* linear sieve to fill spf up to new_limit */
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (prime_cnt >= prime_cap) {
                size_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                if (!tmp) abort();
                primes = tmp;
                prime_cap = new_cap;
            }
            primes[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint64_t p = primes[j];
            idx_t prod = i * (idx_t)p;
            if (prod > new_limit) break;
            spf[prod] = p;
            if (p == spf[i]) break;
        }
    }
    spf_limit = new_limit;
}

/* Compute sigma(n) using the spf array.  The result is returned as a
   value of type seq_t. */
static seq_t sigma_of(idx_t n) {
    if (n == 1) return (seq_t)1;
    seq_t res = (seq_t)1;
    idx_t tmp = n;
    while (tmp > 1) {
        uint64_t p = spf[tmp];
        seq_t term_sum = (seq_t)1;
        seq_t pow = (seq_t)1;
        while ((tmp % p) == 0) {
            tmp /= p;
            pow *= (seq_t)p;
            term_sum += pow;
        }
        res *= term_sum;
    }
    return res;
}

/* Push a new term into the terms array.  The array is dynamically
   resized when necessary. */
static void push_term(seq_t value) {
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = value;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but guard against 0 */

    /* Ensure we have enough terms precomputed */
    while (terms_len < (size_t)n) {
        /* Prepare sieve up to next number if needed */
        ensure_spf(scanned + 1);

        /* Compute sigma for the next number */
        seq_t sigma = sigma_of(scanned + 1);

        /* Check divisibility by 6 */
        seq_t rem = sigma % (seq_t)6;
        if (rem != (seq_t)0) {
            /* satisfies condition: add to the sequence list */
            push_term((seq_t)(scanned + 1));
        }
        ++scanned;
    }
    /* Return the requested term (sequence is 1-based) */
    return terms[n - 1];
}
