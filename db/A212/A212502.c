/*
 * A212502  Composite numbers k that divide the imaginary part of
 *          (1+2i)^A201629(k).
 * https://oeis.org/A212502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over composite k >= 4: k is a term iff
 *            Im((1+2i)^A201629(k)) == 0 (mod k), where A201629(k) = k if k
 *            even, k-1 if k == 1 (mod 4), k+1 if k == 3 (mod 4); the power is
 *            computed by binary exponentiation of Gaussian integers mod k
 * Range:     u8 n <= 23, u16 n <= 796, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9992 (2026-11-07)
 * Helpers:   is_prime, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mulmod.h"
#include "../utils/is_prime.h"

static uint64_t *A212502_cache = 0;
static size_t A212502_cnt = 0, A212502_cap = 0;
static uint64_t A212502_next = 4;

static uint64_t A212502_A201629(uint64_t k)
{
    if ((k & 1) == 0) return k;
    return ((k & 3) == 1) ? (k - 1) : (k + 1);
}

static bool A212502_test(uint64_t k)
{
    uint64_t m = A212502_A201629(k);
    uint64_t ra = 1 % k, rb = 0;
    uint64_t ba = 1 % k, bb = 2 % k;
    while (m) {
        if (m & 1) {
            uint64_t x = mulmod(ra, ba, k), y = mulmod(rb, bb, k);
            uint64_t na = (x >= y) ? (x - y) : (x + (k - y));
            uint64_t p = mulmod(ra, bb, k), q = mulmod(rb, ba, k);
            uint64_t nb = (p >= k - q) ? (p - (k - q)) : (p + q);
            ra = na; rb = nb;
        }
        m >>= 1;
        if (m) {
            uint64_t x = mulmod(ba, ba, k), y = mulmod(bb, bb, k);
            uint64_t na = (x >= y) ? (x - y) : (x + (k - y));
            uint64_t p = mulmod(ba, bb, k);
            uint64_t nb = (p >= k - p) ? (p - (k - p)) : (p + p);
            ba = na; bb = nb;
        }
    }
    return rb == 0;
}

seq_t a(idx_t n)
{
    while (A212502_cnt < (size_t)n) {
        uint64_t k = A212502_next++;
        if (is_prime((idx_t)k)) continue;
        if (A212502_test(k)) {
            if (A212502_cnt == A212502_cap) {
                size_t nc = A212502_cap ? A212502_cap * 2 : 1024;
                uint64_t *np = (uint64_t *)realloc(A212502_cache, nc * sizeof(uint64_t));
                if (!np) return 0;
                A212502_cache = np;
                A212502_cap = nc;
            }
            A212502_cache[A212502_cnt++] = k;
        }
    }
    return (seq_t)A212502_cache[n - 1];
}
