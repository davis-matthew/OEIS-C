/*
 * A002396  Inverse of reduced totient function.
 * https://oeis.org/A002396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   search, not a closed form. a(n) = least m with lambda(m) = k,
 *            where k runs through the values taken by the Carmichael function
 *            lambda (A002322), in increasing order. For each k: m is a
 *            product of prime powers p^e with lambda(p^e) | k, and lcm of
 *            those lambda(p^e) must equal k. A knapsack over the...
 * Range:     u8 n <= 23, u16 n <= 607, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a2396_isprime, a2396_minm, a2396_mulsat
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/a2396_isprime.h"
#include "../utils/a2396_minm.h"
#include "../utils/a2396_mulsat.h"

static uint64_t *a2396_res = NULL;
static idx_t a2396_cnt = 0, a2396_cap = 0;
static uint64_t a2396_nextk = 1;

seq_t a(idx_t n) {
    while (a2396_cnt < n) {
        uint64_t m = a2396_minm(a2396_nextk);
        a2396_nextk++;
        if (m == 0) continue;
        if (a2396_cnt == a2396_cap) {
            idx_t nc = a2396_cap ? a2396_cap * 2 : 1024;
            a2396_res = (uint64_t *)realloc(a2396_res, sizeof(uint64_t) * nc);
            a2396_cap = nc;
        }
        a2396_res[a2396_cnt++] = m;
    }
    return (seq_t)a2396_res[n - 1];
}
