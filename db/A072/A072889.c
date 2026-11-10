/*
 * A072889  Duplicate of A072884.
 * https://oeis.org/A072889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n-th k with f(f(k)) = k, where f(k) = sum of cubes of
 *            decimal digits of k (search over k <= 3000)
 * Range:     i8 n <= 1, i16 n <= 9, i32 n <= 9, i64 n <= 9, i128 n <= 9,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A072884_digit_cube_sum(seq_t v)
{
    seq_t sum = 0;
    while (v > 0) {
        unsigned d = (unsigned)(v % 10);
        seq_t d3 = (seq_t)d * d * d;
        sum += d3;
        v /= 10;
    }
    return sum;
}

static seq_t *A072884_table = NULL;
static size_t A072884_count = 0;
static bool A072884_ready = false;

static void A072884_init_cache(void)
{
    if (A072884_ready) return;

    const idx_t LIMIT = 3000;
    A072884_table = mem_new(seq_t, LIMIT);
    if (!A072884_table) { A072884_ready = true; return; }

    size_t idx = 0;
    for (idx_t n = 1; n <= LIMIT; ++n) {
        seq_t s = A072884_digit_cube_sum((seq_t)n);
        seq_t t = A072884_digit_cube_sum(s);
        if (t == (seq_t)n) {
            A072884_table[idx++] = (seq_t)n;
        }
    }

    A072884_count = idx;
    A072884_ready = true;
}

static seq_t A072884_a(idx_t n)
{
    A072884_init_cache();
    if (n == 0 || n > A072884_count) return 0;
    return A072884_table[n - 1];
}

seq_t a(idx_t n)
{
    return A072884_a(n);
}
