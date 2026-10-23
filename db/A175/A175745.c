/*
 * A175745  Numbers with 35 divisors.
 * https://oeis.org/A175745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest integer with exactly 35 divisors. The
 *            numbers are of the form p^34 or p^6*q^4 with p≠q (p,q primes).
 *            The implementation uses a binary search over the value space.
 *            For each threshold it counts how many such numbers are ≤
 *            threshold, employing a pre‑computed prime table up t...
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   floor_sqrt, ipow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/floor_sqrt.h"
#include "../utils/ipow_uint.h"

/* --------------------------------------------------------------------- */
/* Small integer power (used for exponents 4, 6, 34).                      */

/* --------------------------------------------------------------------- */
/* Prime table generation (up to 200 000).                               */
/* --------------------------------------------------------------------- */
#define PRIME_LIMIT 200000

/* Static data used by the sequence routine */
static uint32_t  *primes      = NULL;   /* prime list */
static size_t     prime_cnt   = 0;      /* number of primes */

static void init_primes(void) {
    static bool done = false;
    if (done) return;
    done = true;

    /* Simple sieve of Eratosthenes */
    uint8_t *sieve = (uint8_t *)calloc(PRIME_LIMIT + 1, sizeof(uint8_t));
    if (!sieve) exit(1);

    size_t temp_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            ++temp_cnt;
            if (i * i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i) sieve[j] = 1;
            }
        }
    }

    primes = (uint32_t *)malloc(temp_cnt * sizeof(uint32_t));
    if (!primes) exit(1);
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!sieve[i]) primes[idx++] = (uint32_t)i;
    prime_cnt = temp_cnt;

    free(sieve);
}

/* --------------------------------------------------------------------- */
/* Upper_bound in the prime array.                                      */
/* Returns first index i such that primes[i] > val   (0 ≤ i ≤ prime_cnt).*/
/* --------------------------------------------------------------------- */
static size_t upper_bound_prime(uint64_t val) {
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if ((uint64_t)primes[mid] <= val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* --------------------------------------------------------------------- */
/* Count of numbers with exactly 35 divisors that are ≤ t.               */
/* --------------------------------------------------------------------- */
static size_t count_leq(seq_t t) {
    init_primes();

    size_t count = 0;

    /* 1. Numbers of the form p^34 */
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p34 = ipow_uint((seq_t)primes[i], 34);
        if (p34 > t) break;
        ++count;
    }

    /* 2. Numbers of the form p^6 * q^4   (p < q) */
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p6 = ipow_uint((seq_t)primes[i], 6);
        if (p6 > t) break;

        seq_t div = t / p6;
        seq_t lim = floor_sqrt(floor_sqrt(div));
        uint64_t limit_q = (lim > (seq_t)ULLONG_MAX) ? ULLONG_MAX : (uint64_t)lim;

        size_t idx_limit = upper_bound_prime(limit_q);
        size_t idx_p = i;
        if (idx_limit > idx_p + 1)
            count += idx_limit - idx_p - 1;
    }

    /* 3. Numbers of the form p^4 * q^6   (p < q) */
    for (size_t j = 0; j < prime_cnt; ++j) {
        seq_t q6 = ipow_uint((seq_t)primes[j], 6);
        if (q6 > t) break;

        seq_t div = t / q6;
        seq_t lim = floor_sqrt(floor_sqrt(div));
        uint64_t limit_p = (lim > (seq_t)ULLONG_MAX) ? ULLONG_MAX : (uint64_t)lim;

        size_t idx_limit = upper_bound_prime(limit_p);
        size_t idx_q = j;
        /* count primes p < q such that p <= limit_p */
        count += (idx_limit < idx_q) ? idx_limit : idx_q;
    }

    return count;
}

/* --------------------------------------------------------------------- */
/* The required sequence function.                                       */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* Binary search to find the smallest t with count_leq(t) >= n */
    init_primes();

    seq_t low = 1;
    seq_t high = 2;

    /* Increase upper bound until it contains at least n numbers */
    while (count_leq(high) < (size_t)n) {
        high <<= 1;                           /* high = high * 2 */
    }

    /* Classic binary search */
    while (low < high) {
        seq_t mid = low + (high - low) / 2;
        if (count_leq(mid) >= (size_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}
