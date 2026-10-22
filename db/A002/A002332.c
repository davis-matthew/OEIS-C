/*
 * A002332  Numbers x such that p = x^2 + 2y^2, with prime p = A033203(n).
 * https://oeis.org/A002332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = x where A033203(n) = x^2 + 2*y^2; primes p = 2 or
 *            p == 1,3 (mod 8) are enumerated by trial division and x found by
 *            scanning
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   q2332_isqrt, q2332_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/q2332_isqrt.h"
#include "../utils/q2332_prime.h"

static uint64_t *q2332_list = NULL;
static idx_t q2332_cnt = 0, q2332_cap = 0;
static uint64_t q2332_next = 2;

seq_t a(idx_t n) {
    uint64_t p, x, lim;
    while (q2332_cnt < n) {
        p = q2332_next;
        q2332_next = (p == 2) ? 3 : p + 2;
        if ((p == 2 || p % 8 == 1 || p % 8 == 3) && q2332_prime(p)) {
            if (q2332_cnt == q2332_cap) {
                idx_t nc = q2332_cap ? q2332_cap * 2 : 1024;
                q2332_list = (uint64_t *)realloc(q2332_list, nc * sizeof(uint64_t));
                q2332_cap = nc;
            }
            q2332_list[q2332_cnt++] = p;
        }
    }
    p = q2332_list[n - 1];
    lim = q2332_isqrt(p);
    for (x = 0; x <= lim; x++) {
        uint64_t rem = p - x * x, y2, y;
        if (rem % 2) continue;
        y2 = rem / 2;
        y = q2332_isqrt(y2);
        if (y * y == y2) return (seq_t)x;
    }
    return (seq_t)0;
}
