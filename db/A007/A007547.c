/*
 * A007547  Number of steps to compute n-th prime in PRIMEGAME (slow version).
 * https://oeis.org/A007547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   simulates PRIMEGAME
 *            (17/91,78/85,19/51,23/38,29/33,77/29,95/23,77/19,1/17,11/13,13/11,15/2,1/7,55/1)
 *            from 2, collapsing each trial division by repeated subtraction
 *            into closed form: for candidate N, each divisor test m = N-1
 *            down to the largest proper divisor L+1 costs 2*floor(N/m) + 6N +
 *            2 steps...
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 233, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pg_largest_proper_divisor, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/u128_to_seq.h"
#include "../utils/pg_largest_proper_divisor.h"

typedef unsigned __int128 pg_u128;

static pg_u128 *pg_hits = NULL;
static uint64_t pg_count = 0, pg_cap = 0;
static uint64_t pg_N = 2;
static pg_u128 pg_pos = 5; /* steps taken on entering the first divisor test for N = 2 */

/* sum of floor(N/m) for m in [lo, hi] */
static pg_u128 pg_floor_sum(uint64_t N, uint64_t lo, uint64_t hi) {
    pg_u128 s = 0;
    uint64_t m = lo;
    while (m <= hi) {
        uint64_t q = N / m;
        uint64_t m2 = N / q;
        if (m2 > hi) m2 = hi;
        s += (pg_u128)q * (m2 - m + 1);
        m = m2 + 1;
    }
    return s;
}

static void pg_advance(void) {
    uint64_t N = pg_N;
    uint64_t L = pg_largest_proper_divisor(N);
    if (L + 1 <= N - 1)
        pg_pos += 2 * pg_floor_sum(N, L + 1, N - 1)
                + (pg_u128)(N - 1 - L) * (6 * (pg_u128)N + 2);
    pg_pos += 4 * (pg_u128)N + 2 * (pg_u128)(N / L) + 2;
    if (L == 1) {
        if (pg_count == pg_cap) {
            uint64_t nc = pg_cap ? pg_cap * 2 : 1024;
            pg_hits = (pg_u128 *)realloc(pg_hits, nc * sizeof(pg_u128));
            pg_cap = nc;
        }
        pg_hits[pg_count++] = pg_pos;
    }
    pg_pos += 3 * (pg_u128)N + L + 1;
    pg_N = N + 1;
}

seq_t a(idx_t n) {
    while (pg_count < n) pg_advance();
    return u128_to_seq(pg_hits[n - 1]);
}
