/*
 * A097366  Largest k such that 2^k divides A096421(n).
 * https://oeis.org/A097366
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k such that 2^k divides b(n), where b(1) = 1 and
 *            b(n) = Sum_{1<=j<n, gcd(j,n)=1} b(j)*b(n-j) for n > 1 (b =
 *            A096421; computed mod 2^bits, which keeps the small 2-adic
 *            valuation exact)
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 59, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 77 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A097366_cache = NULL;
static idx_t A097366_done = 0;
static size_t A097366_cap = 0;

static idx_t A097366_gcd(idx_t a, idx_t b) {
    while (b) { idx_t t = a % b; a = b; b = t; }
    return a;
}

static void A097366_compute(idx_t n) {
    if (n <= A097366_done) return;
    if (A097366_cap < (size_t)n + 1) {
        size_t nc = (size_t)n + 1;
        if (nc < 16) nc = 16;
        A097366_cache = mem_resize(seq_t, A097366_cache, nc);
        A097366_cap = nc;
    }
    if (A097366_done == 0) {
        A097366_cache[0] = 0;
        A097366_cache[1] = 1;
        A097366_done = 1;
    }
    for (idx_t i = A097366_done + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= i / 2; ++j) {
            if (A097366_gcd(j, i) == 1) {
                seq_t prod = A097366_cache[j] * A097366_cache[i - j];
                if (j < i - j) sum += prod * 2;
                else sum += prod;
            }
        }
        A097366_cache[i] = sum;
    }
    A097366_done = n;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    A097366_compute(n);
    seq_t v = A097366_cache[n];
    idx_t k = 0;
    while (v != 0 && (v & 1) == 0) { v >>= 1; ++k; }
    return (seq_t)k;
}
