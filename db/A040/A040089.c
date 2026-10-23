/*
 * A040089  Primes p such that x^3 = 22 has a solution mod p.
 * https://oeis.org/A040089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes p such that x^3 = 22 has a solution modulo p by searching primes */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

static uint64_t *prime_uint64 = NULL;
static size_t prime_cnt = 0, prime_cap = 0;
static seq_t *vals = NULL;
static size_t val_cnt = 0, val_cap = 0;
static uint64_t cur = 2;

static void ensure_cap_seq(size_t *cnt, size_t *cap, seq_t **arr) {
    if (*cnt >= *cap) {
        size_t newcap = (*cap == 0) ? 16 : *cap * 2;
        *arr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

static void ensure_cap_uint64(size_t *cnt, size_t *cap, uint64_t **arr) {
    if (*cnt >= *cap) {
        size_t newcap = (*cap == 0) ? 16 : *cap * 2;
        *arr = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
        *cap = newcap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* sequence defined for n >= 1 */
    while (val_cnt < (size_t)n) {
        uint64_t cu = cur;
        bool isprime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_uint64[i];
            if (p * p > cu) break;
            if (cu % p == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            /* store prime for future divisibility checks */
            ensure_cap_uint64(&prime_cnt, &prime_cap, &prime_uint64);
            prime_uint64[prime_cnt++] = cu;

            bool sol = false;
            if (cu == 2 || cu == 3) {
                sol = true;
            } else {
                uint64_t rem = (cu - 1) % 3;
                if (rem == 0) {
                    uint64_t exp = (cu - 1) / 3;
                    seq_t res = mod_pow((seq_t)22, exp, (seq_t)cu);
                    if (res == (seq_t)1) sol = true;
                } else {
                    sol = true;
                }
            }
            if (sol) {
                ensure_cap_seq(&val_cnt, &val_cap, &vals);
                vals[val_cnt++] = (seq_t)cu;
            }
        }
        cur += (cur == 2) ? 1 : 2; /* advance to next candidate prime */
    }
    return vals[n - 1];
}
