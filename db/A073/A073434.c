/*
 * A073434  Primes p such that 2p+1 is prime and there are an even number of
 *          primes in the interval [p+1,2p+1].
 * https://oeis.org/A073434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   append_candidate, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function enumerates primes p such that 2p+1 is prime and the
 * interval [p+1,2p+1] contains an even number of primes (OEIS A073434).
 * It computes terms on demand, memoising all previously found terms. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/append_candidate.h"
#include "../utils/build_primes_upto.h"

/* static state ----------------------------------------------------- */

static seq_t    *seq_cache = NULL;     /* memoised terms   */
static size_t    seq_len   = 0;        /* number of terms  */
static size_t    seq_cap   = 0;        /* capacity of seq_cache */

static uint64_t *p_arr      = NULL;    /* primes up to limit */
static size_t    p_cnt      = 0;       /* number of primes in p_arr */
static idx_t     sieve_limit = 0;     /* current sieve upper bound */
static size_t    p_idx       = 0;      /* next prime to process */

/* --------------------------------------------------------------- */

static bool binary_search_prime(uint64_t val) {
    size_t lo = 0, hi = p_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (p_arr[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < p_cnt && p_arr[lo] == val);
}

static size_t lower_bound_prime(uint64_t x) {
    size_t lo = 0, hi = p_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (p_arr[mid] < x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

static size_t upper_bound_prime(uint64_t x) {
    size_t lo = 0, hi = p_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (p_arr[mid] <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* Ensure that at least n terms are calculated. */
static void ensure_sequence(idx_t n) {
    while (seq_len < n) {
        /* Grow the sieve if we have exhausted the current prime list. */
        if (p_idx >= p_cnt || sieve_limit == 0) {
            if (sieve_limit == 0)   sieve_limit = 1000000;
            else                    sieve_limit <<= 1;

            if (p_arr) free(p_arr);
            uint64_t *new_arr = NULL;
            size_t    new_cnt = 0;
            build_primes_upto((idx_t)sieve_limit, &new_arr, &new_cnt);
            p_arr = new_arr;
            p_cnt = new_cnt;
            /* Keep p_idx pointing into the new array at the same
             * logical position as before (ok because the primes
             * preceding p_idx are identical). */
            if (p_idx > p_cnt) p_idx = p_cnt;
        }

        /* Process primes one by one until we have enough terms. */
        while (p_idx < p_cnt && seq_len < n) {
            uint64_t p  = p_arr[p_idx++];
            uint64_t qp = 2 * p + 1;

            /* Make sure 2p+1 is within the prime list. */
            if (qp > (uint64_t)sieve_limit) {
                while (qp > (uint64_t)sieve_limit) sieve_limit <<= 1;
                if (p_arr) free(p_arr);
                uint64_t *new_arr = NULL;
                size_t    new_cnt = 0;
                build_primes_upto((idx_t)sieve_limit, &new_arr, &new_cnt);
                p_arr = new_arr;
                p_cnt = new_cnt;
                /* p_idx already points to the same logical prime. */
            }

            if (!binary_search_prime(qp)) continue; /* 2p+1 not prime */

            /* Count primes in [p+1,2p+1] */
            size_t low  = lower_bound_prime(p + 1);
            size_t high = upper_bound_prime(qp);
            size_t count = (high > low) ? (high - low) : 0;

            if ((count & 1) == 0) {
                append_candidate(&seq_cache, &seq_len, &seq_cap, (seq_t)p);
            }
        }
    }
}

/* Public API ------------------------------------------------------ */

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* safety, offset is 1 */
    ensure_sequence(n);
    return seq_cache[n - 1];
}
