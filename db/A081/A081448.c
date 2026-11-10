/*
 * A081448  a(n) = sqrt( A081447(n) )/6.
 * https://oeis.org/A081448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt(A081447(n))/6, where A081447(n) is the smallest
 *            square (6m)^2 such that sum_{k<=n} A081447(k) + 5 is prime;
 *            search over m
 * Range:     u8 n <= 1, u16 n <= 85, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-07)
 * Helpers:   is_prime_u64, mulmod_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod_seq.h"

static bool A081448_prime(seq_t n)
{
    if (n < 2) return false;
    if (sizeof(seq_t) <= 8 || n <= (seq_t)UINT64_MAX)
        return is_prime_u64((uint64_t)n);
    if ((n & 1) == 0) return false;
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    static const int bases[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (int i = 0; i < 12; i++) {
        seq_t b = (seq_t)bases[i];
        if (b >= n) continue;
        seq_t x = 1, p = b % n, e = d;
        while (e > 0) {
            if (e & 1) x = mulmod_seq(x, p, n);
            p = mulmod_seq(p, p, n);
            e >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool ok = false;
        for (unsigned r = 1; r < s; r++) {
            x = mulmod_seq(x, x, n);
            if (x == n - 1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

static uint32_t *A081448_ms = 0;
static idx_t A081448_cnt = 0;
static idx_t A081448_cap = 0;
static seq_t A081448_sum = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A081448_cnt < n) {
        if (A081448_cnt == A081448_cap) {
            idx_t nc = A081448_cap ? A081448_cap * 2 : 256;
            uint32_t *p = (uint32_t *)realloc(A081448_ms, (size_t)nc * sizeof(uint32_t));
            if (!p) return (seq_t)0;
            A081448_ms = p;
            A081448_cap = nc;
        }
        uint32_t m = 1;
        for (;; ++m) {
            seq_t sq = (seq_t)36 * (seq_t)m * (seq_t)m;
            if (A081448_prime(A081448_sum + sq + (seq_t)5)) {
                A081448_sum += sq;
                break;
            }
        }
        A081448_ms[A081448_cnt++] = m;
    }
    return (seq_t)A081448_ms[n - 1];
}
