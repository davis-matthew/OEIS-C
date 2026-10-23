/*
 * A177211  Numbers k that are the products of two distinct primes such that
 *          2*k-1 and 4*k-3 are also products of two distinct primes.
 * https://oeis.org/A177211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, seq_alloc_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A177211 – Numbers k that are the product of two distinct primes such that
 * 2*k-1 and 4*k-3 are also products of two distinct primes.
 * The implementation builds a prime list once and then searches
 * for k in increasing order, caching discovered terms for
 * fast repeated access.  It works with any seq_t / idx_t
 * combination (including arbitrary‑precision seq_t). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* util includes */
#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/seq_alloc_array.h"    /* seq_alloc_array */
#include "../utils/seq_realloc.h"        /* seq_realloc */

/* ---------- prime table ---------- */
static uint64_t *prime_tbl = NULL;
static size_t prime_cnt = 0;
static const uint64_t PRIME_LIMIT = 1000000ULL;   /* sufficient for n≤10000 */

/* build the prime table once */
static void init_primes(void)
{
    if (prime_tbl != NULL)
        return;
    build_primes_upto(PRIME_LIMIT, &prime_tbl, &prime_cnt);
}

/* test whether n is the product of two distinct primes */
static bool is_semiprime_distinct(uint64_t n)
{
    if (n < 6)     /* smallest such number is 2*3=6 */
        return false;

    uint64_t p1 = 0, p2 = 0;
    uint64_t tmp = n;

    /* first prime factor */
    for (size_t i = 0; i < prime_cnt && prime_tbl[i] * prime_tbl[i] <= tmp; ++i) {
        uint64_t p = prime_tbl[i];
        if (tmp % p == 0) {
            p1 = p;
            tmp /= p;
            while (tmp % p == 0) {   /* repeated factor -> not distinct */
                return false;
            }
            break;
        }
    }
    if (p1 == 0)          /* n itself is prime */
        return false;

    /* second prime factor */
    for (size_t i = 0; i < prime_cnt && prime_tbl[i] * prime_tbl[i] <= tmp; ++i) {
        uint64_t p = prime_tbl[i];
        if (tmp % p == 0) {
            p2 = p;
            tmp /= p;
            if (tmp != 1)   /* third factor would exist */
                return false;
            break;
        }
    }
    if (p2 == 0) {                 /* remaining tmp > 1, must be prime */
        if (tmp == 1)               /* only one factor found */
            return false;
        p2 = tmp;
    }

    /* check distinctness */
    return (p1 != p2);
}

/* ---------- sequence cache ---------- */
static seq_t *seq_cache = NULL;
static idx_t seq_len = 0;
static idx_t seq_cap = 0;
static uint64_t current_k = 2;

/* append a new value to the sequence cache */
static void push_seq(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = (seq_cap == 0) ? 4 : seq_cap * 2;
        seq_cache = seq_realloc(seq_cache, seq_len, seq_cap);
    }
    seq_cache[seq_len++] = val;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0 */

    init_primes();

    while (seq_len < n) {
        if (is_semiprime_distinct(current_k)) {
            uint64_t t1 = 2 * current_k - 1;
            uint64_t t2 = 4 * current_k - 3;
            if (is_semiprime_distinct(t1) && is_semiprime_distinct(t2)) {
                push_seq((seq_t)current_k);
            }
        }
        current_k++;
    }
    return seq_cache[n - 1];
}
