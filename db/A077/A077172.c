/*
 * A077172  Triangle formed by grouping the natural numbers so that the n-th
 *          row contains n numbers whose product has exactly n distinct prime
 *          divisors.
 * https://oeis.org/A077172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Computes sequence A077172 using the greedy construction
 *            described in OEIS. For each row of length n it repeatedly takes
 *            the smallest unused natural number that does not cause the count
 *            of distinct prime divisors of the product of numbers in the row
 *            to exceed n. The last number of the row must bri...
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*--------------------------------------------------------------------------*
 *  Global state (static)                                                   *
 *--------------------------------------------------------------------------*/
static uint8_t *used = NULL;          /* 0 = unused, 1 = used          */
static size_t used_cap = 0;

static uint8_t *prime_used = NULL;    /* 0 = not used in current row, 1 = used */
static size_t prime_used_cap = 0;

static uint64_t next_candidate = 1;   /* first number to try in the current search */

static uint32_t *row_primes = NULL;   /* primes that became used in current row */
static size_t row_primes_cap = 0;
static size_t row_primes_len = 0;     /* number of primes stored */

static seq_t *cache = NULL;           /* cached sequence values          */
static size_t cache_cap = 0;
static size_t cache_size = 0;

static uint64_t current_row = 1;      /* how many numbers the current row expects */
static uint64_t row_used_count = 0;   /* how many numbers already placed in row */
static uint64_t distinct_primes_in_row = 0;  /* distinct primes currently used in row */

/*--------------------------------------------------------------------------*
 *  Helper functions                                                        *
 *--------------------------------------------------------------------------*/
/* Ensure that the arrays are large enough to index at least min_cap-1. */
static void ensure_cap(size_t min_cap) {
    if (min_cap <= used_cap) return;

    size_t new_cap = used_cap ? used_cap << 1 : 1024;
    while (new_cap <= min_cap) new_cap <<= 1;

    uint8_t *new_used = (uint8_t *)realloc(used, new_cap);
    if (!new_used) abort();
    if (new_used != used) used = new_used;
    if (new_cap > used_cap) memset(used + used_cap, 0, new_cap - used_cap);
    used_cap = new_cap;

    if (prime_used_cap < used_cap) {
        uint8_t *new_prime_used = (uint8_t *)realloc(prime_used, used_cap);
        if (!new_prime_used) abort();
        if (new_prime_used != prime_used) prime_used = new_prime_used;
        if (used_cap > prime_used_cap) memset(prime_used + prime_used_cap, 0, used_cap - prime_used_cap);
        prime_used_cap = used_cap;
    }
}

/* Return the smallest unused natural number ≥ start. */
static uint64_t find_next_unused(uint64_t start) {
    uint64_t x = start;
    while (1) {
        ensure_cap((size_t)x + 1);
        if (!used[(size_t)x]) return x;
        ++x;
    }
}

/* Factor n and store its distinct prime divisors in out[]. Return count. */
static int factor_distinct_primes(uint64_t n, uint32_t *out, int max_out) {
    int cnt = 0;
    if (n % 2 == 0) {
        if (cnt < max_out) out[cnt++] = 2;
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            if (cnt < max_out) out[cnt++] = (uint32_t)p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) {
        if (cnt < max_out) out[cnt++] = (uint32_t)n;
    }
    return cnt;
}

/*--------------------------------------------------------------------------*
 *  Main function                                                          *
 *--------------------------------------------------------------------------*/
seq_t a(idx_t n) {
    /* one‑time initialization */
    static int initialized = 0;
    if (!initialized) {
        used = NULL;
        prime_used = NULL;
        next_candidate = 1;

        cache = NULL;

        ensure_cap(2);              /* allocate minimal capacity */

        row_primes_cap = 1024;
        row_primes = (uint32_t *)malloc(row_primes_cap * sizeof(uint32_t));
        if (!row_primes) abort();
        row_primes_len = 0;

        current_row = 1;
        row_used_count = 0;
        distinct_primes_in_row = 0;
        cache_cap = 0;
        cache_size = 0;
        initialized = 1;
    }

    while ((idx_t)cache_size < n) {
        /* If the current row is complete, start a new one */
        if (row_used_count == current_row) {
            for (size_t i = 0; i < row_primes_len; ++i) {
                uint32_t p = row_primes[i];
                prime_used[(size_t)p] = 0;
            }
            row_primes_len = 0;
            row_used_count = 0;
            distinct_primes_in_row = 0;
            ++current_row;
            next_candidate = 1;   /* start searching from the smallest unused number again */
        }

        uint64_t cand = find_next_unused(next_candidate);

        /* Factor the candidate */
        uint32_t cand_primes[64];
        int cand_cnt = factor_distinct_primes(cand, cand_primes, 64);

        /* Count how many new distinct primes this candidate would introduce */
        int new_primes = 0;
        for (int i = 0; i < cand_cnt; ++i) {
            uint32_t p = cand_primes[i];
            if (!prime_used[(size_t)p]) ++new_primes;
        }

        uint64_t new_total = distinct_primes_in_row + new_primes;

        if (row_used_count + 1 == current_row) {   /* last position in the row */
            if (new_total != current_row) {
                /* Reject, try next unused number */
                next_candidate = cand + 1;
                continue;
            }
        } else {                                    /* intermediate position */
            if (new_total > current_row) {
                next_candidate = cand + 1;
                continue;
            }
        }

        /* Accept the candidate */
        used[(size_t)cand] = 1;
        next_candidate = cand + 1;

        /* Record new primes used in this row */
        for (int i = 0; i < cand_cnt; ++i) {
            uint32_t p = cand_primes[i];
            if (!prime_used[(size_t)p]) {
                prime_used[(size_t)p] = 1;
                if (row_primes_len == row_primes_cap) {
                    size_t new_cap = row_primes_cap ? row_primes_cap << 1 : 256;
                    uint32_t *new_arr = (uint32_t *)realloc(row_primes,
                                                             new_cap * sizeof(uint32_t));
                    if (!new_arr) abort();
                    row_primes = new_arr;
                    row_primes_cap = new_cap;
                }
                row_primes[row_primes_len++] = p;
                ++distinct_primes_in_row;
            }
        }

        /* Cache the result */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 256;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort();
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_size++] = (seq_t)cand;

        ++row_used_count;
    }

    return cache[(size_t)n - 1];
}
