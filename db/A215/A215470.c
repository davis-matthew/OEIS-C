/*
 * A215470  Prime intersections in a square spiral with positive integers:
 *          primes p such that there are four primes among eight nearest
 *          neighbors of p.
 * https://oeis.org/A215470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p in the Ulam spiral (starting at 1 in the
 *            centre, moving right, then up, left, down, …) such that exactly
 *            four of the eight immediate neighbours of p are also prime.
 *            Computed by generating the spiral and collecting qualifying
 *            primes lazily
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   is_prime, spiral_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"     /* provides: bool is_prime(idx_t) */
#include "../utils/spiral_value.h"

/* Count how many of the eight neighbours of (x,y) are prime. */
static int count_prime_neighbors(int64_t x, int64_t y)
{
    static const int d[3] = {-1, 0, 1};
    int cnt = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (d[i] == 0 && d[j] == 0) continue;
            seq_t v = spiral_value(x + d[i], y + d[j]);
            if (is_prime((idx_t)v)) ++cnt;
        }
    }
    return cnt;
}

/* -------------------------------------------------------------
   Sequence generator
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, no value for n==0 */

    /* Static state for caching results and for the spiral walk. */
    static bool init = false;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0, cache_len = 0;

    /* Spiral traversal state */
    static seq_t num = 1;         /* value at current coordinate */
    static int64_t x = 0, y = 0;  /* current coordinate          */
    static int dir = 0;           /* 0=right,1=up,2=left,3=down   */
    static int step_len = 1;      /* current step length           */
    static int steps_taken = 0;   /* how many steps done this side */

    if (!init) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        init = true;
    }

    /* Grow cache when required. */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out of memory – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Extend the spiral until we have at least n qualifying primes. */
    while (cache_len < n) {
        if (is_prime((idx_t)num) &&
            count_prime_neighbors(x, y) == 4) {
            cache[cache_len++] = num;
        }

        /* Move to the next cell in the Ulam spiral. */
        static const int dx[4] = {1, 0, -1, 0};
        static const int dy[4] = {0, 1, 0, -1};

        x += dx[dir];
        y += dy[dir];
        ++num;

        ++steps_taken;
        if (steps_taken == step_len) {
            steps_taken = 0;
            dir = (dir + 1) & 3;
            if (dir == 0 || dir == 2) ++step_len;   /* increase step after right or left */
        }
    }

    return cache[n - 1];
}
