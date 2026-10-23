/*
 * A144147  A positive integer k is included if every exponent in the prime
 *          factorization of k is coprime to k and if the sum of these
 *          prime-factorization exponents divides k.
 * https://oeis.org/A144147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A144147: a(n) is the n‑th positive integer k such that
 * every exponent in the prime factorization of k is coprime to k
 * and the sum of those exponents divides k. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: Euclidean gcd for seq_t (works for all supported seq_t types). */
static seq_t seq_gcd(seq_t a, seq_t b) {
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Factorization: fills exps[0..cnt-1] with the exponents in the prime factorization of n. */
static void factor_exponents(uint64_t n, uint64_t exps[], size_t *cnt) {
    size_t c = 0;
    uint64_t tmp = n;

    /* handle factor 2 */
    uint64_t e = 0;
    while ((tmp & 1ULL) == 0) {
        e++;
        tmp >>= 1;
    }
    if (e > 0) {
        exps[c++] = e;
    }

    /* handle odd factors */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        e = 0;
        while (tmp % p == 0) {
            e++;
            tmp /= p;
        }
        if (e > 0) {
            exps[c++] = e;
        }
    }

    /* remaining prime > sqrt(original n) */
    if (tmp > 1) {
        exps[c++] = 1;
    }

    *cnt = c;
}

/* Test whether a candidate k satisfies the sequence condition. */
static bool qualifies(uint64_t k) {
    uint64_t exps[64];
    size_t cnt;
    factor_exponents(k, exps, &cnt);

    seq_t k_seq = (seq_t)k;
    seq_t sum = (seq_t)0;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t e_seq = (seq_t)exps[i];
        if (seq_gcd(e_seq, k_seq) != (seq_t)1) {
            return false;
        }
        sum += e_seq;
    }

    /* sum should divide k */
    return (k_seq % sum) == (seq_t)0;
}

/* Main accessor function for the sequence. */
seq_t a(idx_t n) {
    /* static cache state */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t candidate = 2;   /* start from 2, the first term */

    /* Expand cache as needed until we have at least n elements. */
    while (cache_len < n) {
        uint64_t k = candidate++;
        if (qualifies(k)) {
            seq_t val = (seq_t)k;
            if (cache_len + 1 > cache_cap) {
                /* grow cache */
                size_t new_cap = cache_cap == 0 ? 128 : cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) {
                    /* allocation failed – abort (unlikely in tested ranges) */
                    abort();
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
    }

    return cache[n - 1];
}
