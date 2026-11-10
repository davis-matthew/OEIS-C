/*
 * A075991  Indices where record values of A075989 occur.
 * https://oeis.org/A075991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th h such that A075989(h) > A075989(j) for all 1<=j<h;
 *            A075989(h) = Sum_{k=1..h} (floor(2h/(2k+1)) - floor(2h/(2k+2)));
 *            computed by a cached linear scan
 * Range:     u8 n <= 47, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A075991_f(uint64_t n) {
    uint64_t count = 0, two_n = 2 * n;
    for (uint64_t k = 1; k < n; ++k) {
        uint64_t d = 2 * k + 1;
        count += two_n / d - two_n / (d + 1);
    }
    return count;
}

static uint64_t *A075991_rec = 0;
static size_t A075991_cnt = 0, A075991_cap = 0;
static uint64_t A075991_next = 1;
static uint64_t A075991_max = 0;
static int A075991_started = 0;

seq_t a(idx_t n) {
    while (A075991_cnt < (size_t)n) {
        uint64_t h = A075991_next++;
        uint64_t v = A075991_f(h);
        if (!A075991_started || v > A075991_max) {
            A075991_started = 1;
            A075991_max = v;
            if (A075991_cnt == A075991_cap) {
                A075991_cap = A075991_cap ? A075991_cap * 2 : 256;
                A075991_rec = (uint64_t *)realloc(A075991_rec, A075991_cap * sizeof(uint64_t));
            }
            A075991_rec[A075991_cnt++] = h;
        }
    }
    return (seq_t)A075991_rec[n - 1];
}
