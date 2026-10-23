/*
 * A174167  Number of safe primes between squares of consecutive primes.
 * https://oeis.org/A174167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of safe primes between squares of consecutive primes
 * Range:     u8 n <= 216, u16 n <= 8664, u32 n <= 8664, u64 n <= 8664,
 *            u128 n <= 8664, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
/* Utility to build a list of primes up to a limit. */
#include "../utils/build_primes_upto.h"

/* ----------------------------------------------------------------------
   Static state: primes array and its size.
   The array contains the first `prime_cnt` primes (0‑based indexing).
   ---------------------------------------------------------------------- */
static uint64_t *prime_list   = NULL;
static size_t    prime_cnt    = 0;
static size_t    prime_limit  = 0;   /* maximum value of primes checked */

/* ----------------------------------------------------------------------
   Ensure that prime_list contains at least `need` primes (0‑based).
   ---------------------------------------------------------------------- */
static void
ensure_primes_count(idx_t need)
{
    if (prime_cnt > need)
        return;

    /* Start from a reasonable bound and double until enough primes are found. */
    size_t limit = prime_limit ? prime_limit : 100;
    while (prime_cnt <= need) {
        limit *= 2;
        uint64_t *tmp;
        size_t tmp_cnt;
        build_primes_upto(limit, &tmp, &tmp_cnt);

        free(prime_list);
        prime_list   = tmp;
        prime_cnt    = tmp_cnt;
        prime_limit  = limit;
    }
}

/* ----------------------------------------------------------------------
   Test primality of a 64‑bit integer using the precomputed prime list.
   Only primes up to sqrt(x) are required, which are all present in
   prime_list because we precompute well beyond the needed bound.
   ---------------------------------------------------------------------- */
static inline bool
is_prime_uint64(uint64_t x)
{
    if (x < 2)
        return false;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= x; i++) {
        if (x % prime_list[i] == 0)
            return false;
    }
    return true;
}

/* ----------------------------------------------------------------------
   Main sequence function.
   ---------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    /* 1‑based index: a(1) corresponds to first pair of primes (2,3). */
    /* Ensure we have at least n+1 primes. */
    ensure_primes_count(n);           /* we need prime[n]   */
    ensure_primes_count(n + 1);       /* we need prime[n+1] */

    uint64_t p_n     = prime_list[n - 1];     /* p_n  (n‑th prime)  */
    uint64_t p_next  = prime_list[n];        /* p_{n+1} */

    uint64_t low  = p_n * p_n;               /* lower bound of interval     */
    uint64_t high = p_next * p_next;         /* upper bound of interval     */

    /* Length of the segment: we skip endpoints because they are composite. */
    size_t seg_len = (size_t)(high - low - 1);
    if (seg_len == 0)
        return 0;

    /* Allocate segment array: 1 = prime candidate, 0 = composite. */
    unsigned char *segment = (unsigned char *)malloc(seg_len);
    if (!segment)
        return 0;  /* memory failure -> zero count (unlikely in tests). */
    memset(segment, 1, seg_len);

    /* Segmented sieve.  For each base prime <= sqrt(high), mark its multiples. */
    for (size_t i = 0; i < prime_cnt; i++) {
        uint64_t p = prime_list[i];
        if (p * p > high)
            break;

        uint64_t start = (low + p - 1) / p * p;   /* first multiple of p >= low */
        if (start < p * p)
            start = p * p;

        for (uint64_t x = start; x < high; x += p) {
            segment[(size_t)(x - low - 1)] = 0;     /* index offset by -1 because we skip low itself */
        }
    }

    /* Count safe primes in (low, high). */
    seq_t count = 0;
    for (size_t i = 0; i < seg_len; i++) {
        if (!segment[i])
            continue;
        uint64_t candidate = low + 1 + i;
        /* Candidate is prime.  Check if it is a safe prime. */
        uint64_t half = (candidate - 1) / 2;
        if (is_prime_uint64(half))
            count++;
    }

    free(segment);
    return count;
}
