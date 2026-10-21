/*
 * A218469  Number of partitions of n into at most three distinct primes
 *          (including 1).
 * https://oeis.org/A218469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of partitions of n into at most three distinct
 *            primes * (including 1). This is computed by counting all
 *            unordered subsets of * the set {1} ∪ primes of size 1, 2, or 3
 *            whose elements sum to n
 * Range:     u8 n <= 376, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Global storage for primes and prime flags, built up to the maximum n
   requested so far. */
static idx_t sieve_limit = 0;
static idx_t *prime_list = NULL;      /* sorted list of primes ≤ sieve_limit */
static size_t prime_count = 0;         /* number of primes */
static bool *prime_flag = NULL;       /* prime_flag[i] == true iff i is prime */

/* Build a sieve up to 'limit' and fill prime_list and prime_flag.  The
 * function is called only when a larger limit than previously seen is
 * requested. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= sieve_limit) return;

    /* Allocate a new prime flag array. */
    bool *new_flag = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    for (idx_t i = 0; i <= limit; ++i) new_flag[i] = true;
    if (limit >= 0) new_flag[0] = false;
    if (limit >= 1) new_flag[1] = false;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (new_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i) new_flag[j] = false;
        }
    }

    /* Count primes to allocate the list. */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i) if (new_flag[i]) ++count;

    idx_t *new_list = NULL;
    if (count > 0) {
        new_list = (idx_t *)malloc(count * sizeof(idx_t));
        size_t idx = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (new_flag[i]) new_list[idx++] = i;
        }
    }

    /* Replace old data. */
    free(prime_flag);
    free(prime_list);
    prime_flag = new_flag;
    prime_list = new_list;
    prime_count = count;
    sieve_limit = limit;
}

/* Test whether x is a member of S = {1} ∪ primes up to sieve_limit. */
static inline bool in_S(idx_t x)
{
    return (x == 1) || (x <= sieve_limit && prime_flag[x]);
}

/* Main function computing a(n). */
seq_t a(idx_t n)
{
    /* Ensure we have primes up to n for membership tests. */
    ensure_primes_upto(n);

    seq_t count = 0;

    /* Size‑1 partitions. */
    if (n == 1 || (n <= sieve_limit && prime_flag[n])) count++;

    /* Size‑2 partitions (unordered distinct pairs). */
    /* Case involving 1. */
    if (n >= 3) {
        idx_t b = n - 1;
        if (b > 1 && (b == 1 || (b <= sieve_limit && prime_flag[b]))) count++;
    }
    /* Pairs of two primes. */
    for (size_t ia = 0; ia < prime_count; ++ia) {
        idx_t a_val = prime_list[ia];
        if (a_val > n / 2) break;          /* ensures a < b */
        idx_t b_val = n - a_val;
        if (b_val > a_val && b_val <= sieve_limit && prime_flag[b_val]) count++;
    }

    /* Size‑3 partitions (unordered distinct triples). */
    /* Triples containing 1 as the smallest element. */
    if (n >= 4) {
        /* a = 1 */
        idx_t a_val = 1;
        idx_t limit_b = (n - a_val - 1) / 2;   /* ensures c > b */
        for (size_t ib = 0; ib < prime_count && prime_list[ib] <= limit_b; ++ib) {
            idx_t b_val = prime_list[ib];
            idx_t c_val = n - a_val - b_val;   /* c_val > b_val by construction */
            if (c_val > b_val && c_val <= sieve_limit && prime_flag[c_val]) count++;
        }
    }

    /* Triples where all three elements are primes. */
    for (size_t ia = 0; ia < prime_count; ++ia) {
        idx_t a_val = prime_list[ia];
        if (a_val > n - 3) break;                 /* need room for b and c */
        idx_t limit_b = (n - a_val - 1) / 2;       /* ensures c > b */
        for (size_t ib = ia + 1; ib < prime_count && prime_list[ib] <= limit_b; ++ib) {
            idx_t b_val = prime_list[ib];
            idx_t c_val = n - a_val - b_val;       /* c_val > b_val by construction */
            if (c_val > b_val && c_val <= sieve_limit && prime_flag[c_val]) count++;
        }
    }

    return count;
}
