/*
 * A103966  Let p(k) = k-th prime. a(n) = smallest prime p(j) such that number
 *          of solutions to p(k)-p(k-1)=m is n, where 1<k<=j.
 * https://oeis.org/A103966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A103966 – Smallest prime with a given gap‑count
   Formula: a(n) = smallest prime p(j) such that some gap m = p(k)-p(k-1)
   has occurred exactly n times among the gaps up to index j. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Ensure an array of seq_t is large enough to hold indexes [0..min_needed] */
static void ensure_seq_array(seq_t **arr, size_t *cap, size_t min_needed)
{
    if (*cap <= min_needed) {
        size_t newcap = (*cap) ? (*cap) * 2 : 16;
        if (newcap <= min_needed) newcap = min_needed + 1;
        *arr = mem_resize(seq_t, *arr, newcap);
        for (size_t i = *cap; i < newcap; ++i) (*arr)[i] = 0;
        *cap = newcap;
    }
}

/* Ensure an array of uint32_t is large enough to hold indexes [0..min_needed] */
static void ensure_uint32_array(uint32_t **arr, size_t *cap, size_t min_needed)
{
    if (*cap <= min_needed) {
        size_t newcap = (*cap) ? (*cap) * 2 : 16;
        if (newcap <= min_needed) newcap = min_needed + 1;
        *arr = (uint32_t *)realloc(*arr, newcap * sizeof(uint32_t));
        for (size_t i = *cap; i < newcap; ++i) (*arr)[i] = 0;
        *cap = newcap;
    }
}

/* Ensure an array of uint64_t is large enough to hold indexes [0..min_needed] */
static void ensure_uint64_array(uint64_t **arr, size_t *cap, size_t min_needed)
{
    if (*cap <= min_needed) {
        size_t newcap = (*cap) ? (*cap) * 2 : 64;
        if (newcap <= min_needed) newcap = min_needed + 1;
        *arr = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
        *cap = newcap;
    }
}

/* Generate the next prime after last */
static uint64_t generate_next_prime(uint64_t last,
                                    uint64_t **primes, size_t *cnt, size_t *cap)
{
    uint64_t candidate = last + 1;
    if (candidate <= 2) candidate = 2;
    if (candidate > 2 && (candidate & 1) == 0) ++candidate;  /* start from odd */

    for (;;) {
        bool prime = true;
        for (size_t i = 0; i < *cnt; ++i) {
            uint64_t p = (*primes)[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { prime = false; break; }
        }
        if (prime) {
            ensure_uint64_array(primes, cap, *cnt);
            (*primes)[*cnt] = candidate;
            ++(*cnt);
            return candidate;
        }
        candidate += 2;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined, return 0 */

    /* static state */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;
    static uint32_t *gap_counts = NULL;
    static size_t gap_cap = 0;
    static seq_t *a_vals = NULL;
    static size_t a_vals_cap = 0;
    static size_t a_max_count = 0;

    /* initialize once */
    if (primes == NULL) {
        primes = (uint64_t *)malloc(64 * sizeof(uint64_t));
        primes_cnt = 1;
        primes_cap = 64;
        primes[0] = 2;                     /* first prime */
        ensure_uint32_array(&gap_counts, &gap_cap, 32); /* initial gap array */
        ensure_seq_array(&a_vals, &a_vals_cap, 32);
        a_vals[0] = 0;   /* index 0 unused */
    }

    /* produce primes until we have filled a[n] */
    while (a_max_count < (size_t)n) {
        uint64_t new_prime = generate_next_prime(primes[primes_cnt-1],
                                                 &primes, &primes_cnt, &primes_cap);
        uint64_t prev_prime = primes[primes_cnt-2];   /* primes_cnt >=2 now */
        uint64_t gap = new_prime - prev_prime;

        /* count this gap */
        ensure_uint32_array(&gap_counts, &gap_cap, (size_t)gap);
        uint32_t new_count = ++gap_counts[gap];

        /* ensure a_vals is large enough to index by new_count */
        if (new_count > a_max_count) {
            a_max_count = new_count;
            ensure_seq_array(&a_vals, &a_vals_cap, (size_t)a_max_count);
        }

        /* record first occurrence of this count */
        if (a_vals[new_count] == 0) {
            a_vals[new_count] = (seq_t)new_prime;
        }
    }

    return a_vals[n];
}
