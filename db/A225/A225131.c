/*
 * A225131  Numerators of the convolutory inverse of the primes of the form 6m
 *          + 1 (A007528).
 * https://oeis.org/A225131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   The sequence a(n) is the convolutional inverse of the *
 *            generating function 1 + Σ p_k x^k where p_k runs over the primes
 *            * ≡ 1 (mod 6). It satisfies * * a(1) = 1 * a(n) = - Σ_{t=2}^{n}
 *            p_t · a(n-t+1) · 7^(t-2) for n ≥ 2 * * where p_t is the t‑th
 *            prime congruent to 1 (mod 6) (p_1 = 7). * * This im...
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 10, i64 n <= 21, i128 n <= 41,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

static uint64_t *primes1mod6 = NULL;
static size_t primes_cap = 0;
static size_t primes_cnt = 0;

static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;
static idx_t seq_max = 0;

static seq_t *pow7_cache = NULL;
static size_t pow7_cap = 0;

/* Ensure that primes1mod6 has at least n entries (p_1 … p_n). */
static void ensure_primes1mod6(idx_t n) {
    while (primes_cnt < (size_t)n) {
        uint64_t start = (primes_cnt == 0) ? 7ULL : primes1mod6[primes_cnt - 1] + 6ULL;
        for (; primes_cnt < (size_t)n; start += 6ULL) {
            if (is_prime_small((seq_t)start)) {
                if (primes_cnt >= primes_cap) {
                    size_t newcap = primes_cap ? primes_cap * 2 : 16;
                    uint64_t *tmp = (uint64_t *)realloc(primes1mod6, newcap * sizeof(uint64_t));
                    primes1mod6 = tmp;
                    primes_cap = newcap;
                }
                primes1mod6[primes_cnt++] = start;
            }
        }
    }
}

/* Ensure pow7_cache stores 7^k for 0 ≤ k < n. */
static void ensure_pow7(idx_t n) {
    if (!pow7_cache) {
        pow7_cache = (seq_t *)calloc(1, sizeof(seq_t));
        if (!pow7_cache) return;
        pow7_cache[0] = (seq_t)1;
        pow7_cap = 1;
    }
    size_t needlen = (size_t)((n > 1) ? (n - 1) : 1);
    if (pow7_cap < needlen) {
        size_t newcap = pow7_cap;
        while (newcap < needlen) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(pow7_cache, newcap * sizeof(seq_t));
        for (size_t i = pow7_cap; i < newcap; ++i) {
            tmp[i] = tmp[i - 1] * (seq_t)7;
        }
        pow7_cache = tmp;
        pow7_cap = newcap;
    }
}

/* Ensure seq_cache can hold values up to index n. */
static void ensure_seq_cache(idx_t n) {
    if (!seq_cache) {
        seq_cap = (size_t)(n + 1) * 2;
        seq_cache = (seq_t *)calloc(seq_cap, sizeof(seq_t));
        if (!seq_cache) return;
        seq_cache[0] = (seq_t)1;          /* unused */
        seq_cache[1] = (seq_t)1;          /* a(1) = 1 */
        seq_max = 1;
    }
    if (seq_cap <= (size_t)n) {
        size_t newcap = seq_cap;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, newcap * sizeof(seq_t));
        seq_cache = tmp;
        seq_cap = newcap;
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1 */
    ensure_primes1mod6(n);
    ensure_pow7(n);
    ensure_seq_cache(n);

    if (seq_max < n) {
        for (idx_t k = seq_max + 1; k <= n; ++k) {
            seq_t sum = (seq_t)0;
            for (idx_t t = 2; t <= k; ++t) {
                seq_t prod = seq_cache[k - t + 1];
                prod *= (seq_t)primes1mod6[t - 1];
                prod *= pow7_cache[t - 2];
                sum += prod;
            }
            seq_cache[k] = -sum;
        }
        seq_max = n;
    }
    return seq_cache[n];
}
