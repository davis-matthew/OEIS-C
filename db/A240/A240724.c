/*
 * A240724  Primes arising in A062391.
 * https://oeis.org/A240724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A062391(n) + A062391(n+1) + A062391(n+2), where
 *            A062391(1)=3, A062391(2)=5 and each later term is found by
 *            searching for the smallest prime > previous term whose sum with
 *            the two previous terms is prime
 * Range:     u8 n <= 13, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static uint64_t *A062391_c = 0;
static size_t A062391_len = 0;
static size_t A062391_cap = 0;

static void A062391_push(uint64_t v)
{
    if (A062391_len == A062391_cap) {
        size_t nc = A062391_cap ? A062391_cap * 2 : 64;
        uint64_t *np = (uint64_t *)realloc(A062391_c, nc * sizeof(uint64_t));
        if (!np) return;
        A062391_c = np;
        A062391_cap = nc;
    }
    A062391_c[A062391_len++] = v;
}

static void A062391_extend(size_t m)
{
    if (A062391_len == 0) {
        A062391_push(3);
        A062391_push(5);
    }
    while (A062391_len < m) {
        uint64_t p = A062391_c[A062391_len - 1];
        uint64_t pp = A062391_c[A062391_len - 2];
        uint64_t cand = p + 2;
        while (!(is_prime((idx_t)cand) && is_prime((idx_t)(pp + p + cand))))
            cand += 2;
        A062391_push(cand);
    }
}

seq_t a(idx_t n)
{
    size_t k = (size_t)n;
    A062391_extend(k + 2);
    uint64_t s = A062391_c[k - 1] + A062391_c[k] + A062391_c[k + 1];
    return (seq_t)s;
}
