/*
 * A086145  Triangle read by rows in which T(n,k) is the least positive
 *          integer s such that p divides k^s-1, where p=prime(n) and k ranges
 *          from 1 to p-1.
 * https://oeis.org/A086145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the multiplicative order of k modulo prime(row), where k
 *            = n - sum_{i<row} (prime(i)-1) and row is the first index such
 *            that that sum plus (prime(row)-1) ≥ n. This reproduces the
 *            triangle entries of A086145 read by rows
 * Range:     u8 n <= 5067, u16 n <= 5067, u32 n <= 5067, u64 n <= 5067,
 *            u128 n <= 5067, big any n
 * Helpers:   modpow64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/modpow64.h"

static bool initialized = false;
static uint64_t primes[2000];
static idx_t cum_len[2001];
static size_t prime_cnt = 0;

static void init(void) {
    cum_len[0] = 0;
    for (uint64_t candidate = 2; prime_cnt < 2000; ++candidate) {
        bool is_prime = true;
        for (uint64_t d = 2; d * d <= candidate; ++d) {
            if (candidate % d == 0) {
                is_prime = false;
                break;
            }
        }
        if (!is_prime) continue;
        primes[prime_cnt++] = candidate;
        cum_len[prime_cnt] = cum_len[prime_cnt - 1] + (candidate - 1);
        /* Break early if cum_len exceeds typical max n (~10000) */
        if (cum_len[prime_cnt] >= 20000) break;
    }
}

/* Compute the multiplicative order of k modulo p (p prime). */
static seq_t multiplicative_order(idx_t k, idx_t p) {
    idx_t m = p - 1;
    idx_t order = m;
    idx_t temp = m;
    for (idx_t q = 2; q * q <= temp; ++q) {
        if (temp % q == 0) {
            while (temp % q == 0) temp /= q;
            while (order % q == 0 &&
                   modpow64((uint64_t)k, (uint64_t)(order / q), (uint64_t)p) == 1)
                order /= q;
        }
    }
    if (temp > 1) {
        idx_t q = temp;
        while (order % q == 0 &&
               modpow64((uint64_t)k, (uint64_t)(order / q), (uint64_t)p) == 1)
            order /= q;
    }
    return (seq_t)order;
}

seq_t a(idx_t n) {
    if (!initialized) {
        init();
        initialized = true;
    }
    /* Find the row that contains the nth element. */
    size_t r = 0;
    while (cum_len[r + 1] < n) ++r;
    idx_t p = primes[r];
    idx_t k = n - cum_len[r];
    return multiplicative_order(k, p);
}
