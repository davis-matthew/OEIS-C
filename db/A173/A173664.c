/*
 * A173664  Sums of 2 primes that are not product of 2 primes.
 * https://oeis.org/A173664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 125, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the nth positive integer that can be expressed as a sum of two
 * primes but is not a product of two primes (offset 1).  
 * The implementation generates integers in increasing order, tests each
 * candidate for the two conditions, and memoizes the results.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/* static state                                                 */
/* ------------------------------------------------------------ */
static uint64_t   prime_limit = 0;          /* highest sieved number   */
static bool      *prime_mark  = NULL;       /* 0 = composite, 1 = prime */
static idx_t     *prime_list  = NULL;       /* list of primes encountered   */
static size_t     prime_cnt   = 0;           /* count of primes in list      */

static seq_t    *seq_cache   = NULL;        /* cached sequence values       */
static size_t     seq_len     = 0;           /* number already stored        */
static size_t     seq_cap     = 0;           /* allocated capacity           */

static uint64_t   last_checked = 0;         /* last tested integer          */

/* ------------------------------------------------------------ */
/* helper: sieve primes up to a given limit (inclusive)        */
/* ------------------------------------------------------------ */
static void sieve_up_to(uint64_t limit)
{
    if (limit == 0) return;

    /* free any previous allocation          */
    if (prime_mark) free(prime_mark);
    if (prime_list) free(prime_list);

    /* create a boolean array for primality   */
    prime_mark = (bool*)calloc(limit + 1, sizeof(bool));
    if (!prime_mark) abort();

    for (uint64_t i = 2; i <= limit; ++i) prime_mark[i] = true;

    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (prime_mark[p]) {
            for (uint64_t q = p * p; q <= limit; q += p)
                prime_mark[q] = false;
        }
    }

    /* gather primes into a contiguous array   */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (prime_mark[i]) ++cnt;

    prime_list = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!prime_list) abort();

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (prime_mark[i]) prime_list[idx++] = i;

    prime_cnt  = cnt;
    prime_limit = limit;
}

/* ------------------------------------------------------------ */
/* ensure primes are available up to at least 'min_limit'       */
/* if prime_limit < min_limit, extend the sieve up to a larger
 * limit that is at least min_limit (roughly double the current limit)
 * ------------------------------------------------------------ */
static void ensure_primes_at_least(uint64_t min_limit)
{
    if (prime_limit >= min_limit) return;

    uint64_t new_limit;
    if (prime_limit == 0) {
        new_limit = min_limit;
    } else {
        /* double until we exceed min_limit, take care of overflow   */
        new_limit = prime_limit;
        while (new_limit < min_limit) {
            if (new_limit > UINT64_MAX / 2) { new_limit = min_limit; break; }
            new_limit *= 2;
        }
    }

    sieve_up_to(new_limit);
}

/* ------------------------------------------------------------ */
/* check if n can be written as a sum of two primes             */
/* ------------------------------------------------------------ */
static bool can_be_sum_of_two_primes(uint64_t n)
{
    if (n < 4) return false;          /* smallest sum is 2+2 */

    ensure_primes_at_least(n);

    if (n & 1ULL) {                    /* odd: must be 2 + (n-2) */
        uint64_t p = n - 2;
        return p >= 2 && prime_mark[p];
    }

    /* even: test Goldbach partitions */
    uint64_t half = n / 2;
    for (size_t i = 0; i < prime_cnt && prime_list[i] <= half; ++i) {
        uint64_t p = prime_list[i];
        uint64_t q = n - p;
        if (q <= prime_limit && prime_mark[q]) return true;
    }
    return false;
}

/* ------------------------------------------------------------ */
/* check if n is the product of exactly two primes (semiprime) */
/* ------------------------------------------------------------ */
static bool is_semiprime(uint64_t n)
{
    if (n < 4) return false;

    /* need primes up to sqrt(n) and n itself for quick marking */
    ensure_primes_at_least(n);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > n / p) break;          /* p > sqrt(n)  */
        if (n % p == 0) {
            uint64_t q = n / p;
            if (q >= 2 && q <= prime_limit && prime_mark[q]) return true;
        }
    }
    return false;
}

/* ------------------------------------------------------------ */
/* main sequence accessor                                     */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined; offset is 1 */

    /* generate terms until we have at least n terms */
    while (seq_len < (size_t)n) {
        uint64_t cand = last_checked + 1;
        last_checked = cand;

        /* ensure we can test cand */
        ensure_primes_at_least(cand);

        if (!is_semiprime(cand) && can_be_sum_of_two_primes(cand)) {
            /* grow cache if necessary */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 4;
                seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                seq_cache = tmp;
                seq_cap   = new_cap;
            }
            seq_cache[seq_len++] = (seq_t)cand;
        }
    }

    return seq_cache[n - 1];
}
