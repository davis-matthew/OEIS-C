/*
 * A089019  First nontrivial x satisfying x^2 - 3*y^2 = d^2, where
 *          d=A072330(n).
 * https://oeis.org/A089019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest x > d with gcd(x,d) = 1 and x^2 - d^2 = 3*y^2
 *            for some integer y, where d = A072330(n) (found by search over x
 *            = d+k)
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 944 (2026-11-07)
 * Helpers:   gcd, is_square, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"
#include "../utils/is_square.h"
#include "../utils/gcd.h"

static bool A089019_ok(uint64_t m)
{
    uint64_t p;
    if (m <= 1) return true;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t r = p % 12;
            if (r != 1 && r != 11) return false;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) {
        uint64_t r = m % 12;
        if (r != 1 && r != 11) return false;
    }
    return true;
}

static uint64_t *A089019_d = NULL;
static size_t A089019_len = 0, A089019_cap = 0;
static uint64_t A089019_last = 0;

static uint64_t A089019_get(idx_t n)
{
    while (A089019_len < (size_t)n) {
        A089019_last++;
        if (A089019_ok(A089019_last)) {
            if (A089019_len >= A089019_cap) {
                size_t nc = A089019_cap ? A089019_cap * 2 : 64;
                A089019_d = (uint64_t *)realloc(A089019_d, nc * sizeof(uint64_t));
                if (!A089019_d) exit(1);
                A089019_cap = nc;
            }
            A089019_d[A089019_len++] = A089019_last;
        }
    }
    return A089019_d[n - 1];
}

seq_t a(idx_t n)
{
    seq_t d = (seq_t)A089019_get(n);
    seq_t k;
    for (k = 1;; k++) {
        /* primitive solution: gcd(x, d) = 1 with x = d + k, i.e. gcd(k, d) = 1 */
        if (gcd(k, d) != 1) continue;
        {
            seq_t t = k * (k + 2 * d);
            if (t % 3 == 0 && is_square(t / 3))
                return d + k;
        }
    }
}
