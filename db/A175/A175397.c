/*
 * A175397  Numbers such that both arithmetic means of distinct and all prime
 *          factors are not integers.
 * https://oeis.org/A175397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A175397: Numbers such that both arithmetic means of distinct and
   all prime factors are not integers.
   Computation by incremental search with a sieve‑based smallest prime
   factor table. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cached sequence values (lazy generation). */
static seq_t *seq = NULL;          /* array of sequence values */
static size_t seq_len = 0;         /* how many terms stored */
static size_t seq_cap = 0;         /* allocated capacity */

/* Last integer that has been considered in the search. */
static uint64_t last_checked = 0;

/* Smallest Prime Factor (SPF) table. */
static uint64_t *spf = NULL;
static uint64_t  spf_size = 0;

/* Grow SPF table to include at least 'limit' (inclusive). */
static void
ensure_spf(uint64_t limit)
{
    if (limit <= spf_size)
        return;

    uint64_t new_size = spf_size == 0 ? 256 : spf_size;
    while (new_size < limit)
        new_size <<= 1;

    uint64_t *new_spf = (uint64_t *)malloc((new_size + 1) * sizeof(uint64_t));
    for (uint64_t i = 0; i <= new_size; ++i)
        new_spf[i] = 0;

    /* Sieve for smallest prime factor. */
    for (uint64_t i = 2; i * i <= new_size; ++i) {
        if (new_spf[i] == 0) {          /* i is prime */
            new_spf[i] = i;
            for (uint64_t j = i * i; j <= new_size; j += i) {
                if (new_spf[j] == 0)
                    new_spf[j] = i;
            }
        }
    }

    /* Remaining numbers are primes. */
    for (uint64_t i = 2; i <= new_size; ++i)
        if (new_spf[i] == 0)
            new_spf[i] = i;

    free(spf);
    spf = new_spf;
    spf_size = new_size;
}

/* Return true if m satisfies the sequence condition. */
static bool
qualifies(uint64_t m)
{
    if (m == 1)
        return true;                   /* 1 is included by definition */

    uint64_t x = m;
    seq_t sum_total = 0;
    int   total_cnt = 0;
    seq_t sum_distinct = 0;
    int   distinct_cnt = 0;
    uint64_t last_prime = 0;

    while (x > 1) {
        uint64_t p = spf[x];
        if (p == 0)        /* safety for large primes */
            p = x;

        if (p != last_prime) {
            sum_distinct += p;
            ++distinct_cnt;
            last_prime = p;
        }

        sum_total += p;
        ++total_cnt;

        x /= p;
    }

    /* Check arithmetic means. */
    if (sum_total % (seq_t)total_cnt == 0)
        return false;
    if (sum_distinct % (seq_t)distinct_cnt == 0)
        return false;

    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined index */

    /* Generate terms until we have the requested one. */
    while (seq_len < (size_t)n) {
        uint64_t candidate = last_checked + 1;
        ensure_spf(candidate);

        if (qualifies(candidate)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap == 0 ? 16 : seq_cap << 1;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len] = (seq_t)candidate;
            ++seq_len;
        }
        last_checked = candidate;
    }

    return seq[n - 1];
}
