/*
 * A024563  a(n) = [ n/{n*sqrt(7)} ], where {x} := x - [ x ].
 * https://oeis.org/A024563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ n / { n*√7 } ⌋
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses integer arithmetic only, relying on floor_sqrt(seq_t). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Handle n = 0 just in case (offset 1 is required) */
    if (n == 0) return 0;

    /* Convert index to seq_t for arithmetic */
    seq_t N = (seq_t)n;

    /* s = 7 * n^2 */
    seq_t s = N * N;
    s = s * (seq_t)7;

    /* g = floor_sqrt(s) = floor(n*√7) */
    seq_t g = floor_sqrt(s);

    /* t = s - g^2 (always > 0 for n >= 1) */
    seq_t t = s - g * g;

    /* Initial guess k0 = floor( 2 * g * n / t ) */
    seq_t k = (g * N * (seq_t)2) / t;
    if (k == 0) k = 1;          /* ensure a(n) >= 1 */

    /* Search for the correct k satisfying the inequalities:
     * (N + k*g)^2 >= 7 * k^2 * N^2
     * (N + (k+1)*g)^2 < 7 * (k+1)^2 * N^2
     *
     * These are equivalent to k = ⌊ n / {n*√7} ⌋.
     */
    for (;;)
    {
        /* Left inequality */
        seq_t left_num = N + k * g;
        left_num = left_num * left_num;
        seq_t left_den = (seq_t)7 * k * k * N * N;
        if (left_num < left_den) {
            /* Inequality fails: decrease k */
            k = k - 1;
            if (k == 0) k = 1;  /* keep k positive */
            continue;
        }

        /* Right inequality */
        seq_t k1 = k + 1;
        seq_t right_num = N + k1 * g;
        right_num = right_num * right_num;
        seq_t right_den = (seq_t)7 * k1 * k1 * N * N;

        if (right_num < right_den) {
            /* Both inequalities hold: k is the answer */
            return k;
        }

        /* Right inequality fails: increase k */
        k = k1;
    }
}
