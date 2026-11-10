/*
 * A057757  Least nonnegative square root of 2 mod n for n in A057126.
 * https://oeis.org/A057757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = least x >= 0 with x^2 == 2 (mod k), where k = A057126(n)
 *            is the n-th number k such that 2 is a square mod k (found by
 *            search/brute force)
 * Range:     u8 n <= 103, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 932 (2026-11-07)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static uint64_t *A057757_ks = NULL;
static size_t A057757_size = 0;
static size_t A057757_cap = 0;
static uint64_t A057757_next = 1;

static bool A057757_valid(uint64_t k)
{
    if (k % 4 == 0) return false;
    if (k == 1) return true;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if (p == 2) continue;
        if ((p % 8ULL) != 1ULL && (p % 8ULL) != 7ULL) return false;
    }
    return true;
}

static uint64_t A057757_kth(idx_t n)
{
    size_t need = (size_t)n;
    while (A057757_size < need) {
        if (A057757_valid(A057757_next)) {
            if (A057757_size == A057757_cap) {
                size_t nc = A057757_cap ? A057757_cap * 2 : 128;
                uint64_t *p = (uint64_t *)realloc(A057757_ks, nc * sizeof(uint64_t));
                if (!p) return 0;
                A057757_ks = p;
                A057757_cap = nc;
            }
            A057757_ks[A057757_size++] = A057757_next;
        }
        ++A057757_next;
    }
    return A057757_ks[need - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t k = A057757_kth(n);
    if (k <= 2) return 0;
    uint64_t target = 2 % k;
    for (uint64_t x = 0; x < k; ++x) {
        unsigned __int128 sq = (unsigned __int128)x * x;
        if ((uint64_t)(sq % k) == target) return (seq_t)x;
    }
    return 0;
}
