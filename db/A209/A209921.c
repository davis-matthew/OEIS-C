/*
 * A209921  Position of positive values in A209661 and A209662.
 * https://oeis.org/A209921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer m >= 1 such that Omega_{1 mod 4}(m) is even
 *            (number of prime factors == 1 mod 4, with multiplicity);
 *            computed by scanning m with trial division and caching the
 *            positions found
 * Range:     u8 n <= 148, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9702 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A209921_list = NULL;
static size_t A209921_cnt = 0;
static size_t A209921_cap = 0;
static uint64_t A209921_next = 1;

static bool A209921_positive(uint64_t m)
{
    unsigned parity = 0;
    for (uint64_t p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e ^= 1; }
            if ((p & 3) == 1) parity ^= e;
        }
    }
    if (m > 1 && (m & 3) == 1) parity ^= 1;
    return parity == 0;
}

seq_t a(idx_t n)
{
    size_t need = (size_t)n;
    while (A209921_cnt < need) {
        if (A209921_positive(A209921_next)) {
            if (A209921_cnt == A209921_cap) {
                size_t nc = A209921_cap ? A209921_cap * 2 : 1024;
                A209921_list = (uint64_t *)realloc(A209921_list, nc * sizeof(uint64_t));
                A209921_cap = nc;
            }
            A209921_list[A209921_cnt++] = A209921_next;
        }
        A209921_next++;
    }
    return (seq_t)A209921_list[need - 1];
}
