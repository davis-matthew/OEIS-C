/*
 * A141115  Those positive integers k where both d(d(k)) is not coprime to k
 *          and d(d(k)) does not divide k, where d(k) is the number of
 *          divisors of k.
 * https://oeis.org/A141115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer k such that d(d(k)) shares a
 *            common divisor > 1 with k and d(d(k)) does not divide k
 * Range:     u8 n <= 32, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Static storage for the sequence and the divisor counting cache */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;
static uint64_t last_checked = 0;

/* Count divisors of an unsigned 64‑bit integer using trial division. */
static inline uint64_t divisor_count_uint64(uint64_t n) {
    if (n < 2) return 1;
    uint64_t cnt = 1;
    uint64_t exp = 0;
    while ((n & 1) == 0) {
        n >>= 1;
        exp++;
    }
    if (exp) cnt *= (exp + 1);
    for (uint64_t p = 3; p * p <= n; p += 2) {
        exp = 0;
        while (n % p == 0) {
            n /= p;
            exp++;
        }
        if (exp) cnt *= (exp + 1);
    }
    if (n > 1) cnt *= 2;
    return cnt;
}

/* Wrapper to return a seq_t value. */
static inline seq_t divisor_count_seq(seq_t x) {
    return (seq_t)divisor_count_uint64((uint64_t)x);
}

/* Test whether a positive integer k satisfies the sequence condition. */
static inline bool test_k(uint64_t k) {
    seq_t d_k = divisor_count_seq(k);
    seq_t m   = divisor_count_seq(d_k);          /* d(d(k)) */
    if (gcd(m, (seq_t)k) <= 1) return false;    /* not coprime */
    if ((k % (uint64_t)m) == 0) return false;    /* m divides k */
    return true;
}

/* Return the nth term of sequence A141115. */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* sequence is 1-indexed */
    while (seq_len < n) {
        last_checked++;
        if (test_k(last_checked)) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)last_checked;
        }
    }
    return seq_cache[n - 1];
}
