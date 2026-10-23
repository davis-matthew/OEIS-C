/*
 * A132414  Integers n such that n^3 - (n + 2)^2 + n + 4 is a square.
 * https://oeis.org/A132414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 none, i32 n <= 4, i64 n <= 5, i128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A132414: a(n) is the nth integer n such that
   n^3 - (n + 2)^2 + n + 4 (= n^3 - n^2 - 3n) is a perfect square. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root using binary search (works for any seq_t) */
static seq_t isqrt(seq_t x) {
    if (x < 0) return 0;
    seq_t low = 0, high = x;
    while (low <= high) {
        seq_t mid = low + (high - low) / 2;
        seq_t sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            if (mid + 1 > high) return mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* Cache storage for the discovered integer solutions */
static seq_t *solutions = NULL;
static size_t sol_count = 0;
static size_t sol_cap   = 0;

/* Compute all integer solutions of n^3 - n^2 - 3n being a square
   within a sufficiently large bound.  The known solutions are
   -1, 0, 3, 4, 75; this bound is intentionally large to be safe. */
static void compute_solutions(void) {
    if (sol_count > 0) return; /* already computed */

    const seq_t LIMIT = (seq_t)10000; /* ample bound */
    for (seq_t n = -LIMIT; n <= LIMIT; ++n) {
        seq_t val = n * n * n - n * n - 3 * n;
        if (val < 0) continue;
        seq_t r = isqrt(val);
        if (r * r == val) {
            if (sol_count == sol_cap) {
                sol_cap = sol_cap ? sol_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(solutions, sol_cap * sizeof(seq_t));
                if (!tmp) break;
                solutions = tmp;
            }
            solutions[sol_count++] = n;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset starts at 1 */
    compute_solutions();
    return (n <= sol_count) ? solutions[n - 1] : (seq_t)0;
}
