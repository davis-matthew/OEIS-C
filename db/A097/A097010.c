/*
 * A097010  Numbers n such that zero is eventually reached when the map x ->
 *          A034460(x) is iterated, starting from x = n.
 * https://oeis.org/A097010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number k such that iterating x -> A034460(x) =
 *            usigma(x) - x from x = k reaches 0 (found by direct search with
 *            trial-division factoring, capped trajectory length and size)
 * Range:     u8 n <= 234, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 88 terms through n = 9974 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A034460_step(uint64_t n)
{
    if (n == 0)
        return 0;
    uint64_t prod = 1, t = n;
    if ((t & 1) == 0) {
        uint64_t pw = 1;
        while ((t & 1) == 0) {
            t >>= 1;
            pw <<= 1;
        }
        prod *= (pw + 1);
    }
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            uint64_t pw = 1;
            while (t % p == 0) {
                t /= p;
                pw *= p;
            }
            prod *= (pw + 1);
        }
    }
    if (t > 1)
        prod *= (t + 1);
    return prod - n;
}

static int A097010_reaches_zero(uint64_t k)
{
    uint64_t x = k;
    for (int i = 0; i < 300; ++i) {
        if (x == 0)
            return 1;
        if (x > ((uint64_t)1 << 40))
            return 0;
        uint64_t y = A034460_step(x);
        if (y == x)
            return 0;
        x = y;
    }
    return 0;
}

static uint64_t *A097010_terms = 0;
static size_t A097010_cnt = 0, A097010_cap = 0;
static uint64_t A097010_next = 1;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    while (A097010_cnt < (size_t)n) {
        if (A097010_reaches_zero(A097010_next)) {
            if (A097010_cnt == A097010_cap) {
                A097010_cap = A097010_cap ? A097010_cap * 2 : 1024;
                A097010_terms = (uint64_t *)realloc(A097010_terms, A097010_cap * sizeof(uint64_t));
            }
            A097010_terms[A097010_cnt++] = A097010_next;
        }
        A097010_next++;
    }
    return (seq_t)A097010_terms[n - 1];
}
