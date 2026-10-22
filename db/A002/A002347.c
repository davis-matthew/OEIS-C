/*
 * A002347  Consider all primes of form p = (x^2 + 11y^2 )/4; sequence gives
 *          values of y.
 * https://oeis.org/A002347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. For each prime p in increasing order, look for y >= 1
 *            such that 4p - 11y^2 is a perfect square x^2. The first prime
 *            that has such a y gives a(1), the next gives a(2), and so on.
 *            The y found is unique because the class number of -11 is 1
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   a2347_isprime, a2347_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/a2347_isprime.h"
#include "../utils/a2347_isqrt.h"

static seq_t *a2347_vals = NULL;
static idx_t a2347_cnt = 0;
static idx_t a2347_cap = 0;
static uint64_t a2347_p = 2;

static void a2347_extend(idx_t upto) {
    if (a2347_cnt >= upto) return;
    if (upto > a2347_cap) {
        idx_t nc = upto * 2 + 16;
        a2347_vals = (seq_t *)realloc(a2347_vals, (size_t)nc * sizeof(seq_t));
        a2347_cap = nc;
    }
    while (a2347_cnt < upto) {
        uint64_t p = a2347_p++;
        uint64_t fp, y;
        if (!a2347_isprime(p)) continue;
        fp = 4 * p;
        for (y = 1; 11 * y * y <= fp; y++) {
            uint64_t xs = fp - 11 * y * y;
            uint64_t x = a2347_isqrt(xs);
            if (x * x == xs) {
                a2347_vals[a2347_cnt++] = (seq_t)y;
                break;
            }
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    a2347_extend(n);
    return a2347_vals[n - 1];
}
