/*
 * A032761  Take list of cubes, move left digit of each term to end of
 *          previous term.
 * https://oeis.org/A032761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = modified cube(n‑1) after moving the leftmost digit of
 *            each subsequent cube to the end of the previous cube
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 1127, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses only integer arithmetic, no external libraries apart from the
   standard utils provided. */

#include <stdlib.h>
#include "../utils/pow10.h"

/* Return the leftmost decimal digit of x (x ≥ 0). */
static seq_t leftmost_digit(seq_t x)
{
    seq_t d = x;
    if (d == 0) return (seq_t)0;
    while (d >= 10) d /= 10;
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    size_t m = (size_t)(n + 1);           /* number of cubes needed */
    seq_t *cubes = (seq_t *)malloc(m * sizeof(seq_t));
    if (!cubes) return (seq_t)0;          /* fall back if allocation fails */

    /* initial cubes list: cube[i] = i^3 */
    for (idx_t i = 0; i < (idx_t)m; ++i) {
        seq_t vi = (seq_t)i;
        cubes[i] = vi * vi * vi;
    }

    /* perform the "move left digit" transformation */
    for (idx_t i = 1; i < (idx_t)m; ++i) {
        seq_t d = leftmost_digit(cubes[i]);          /* left digit of current cube */

        /* append d to the end of previous cube */
        cubes[i - 1] = cubes[i - 1] * 10 + d;

        /* remove the leftmost digit from current cube */
        /* count digits of current cube before removal */
        idx_t digits = 0;
        seq_t temp = cubes[i];
        do { ++digits; temp /= 10; } while (temp > 0);
        /* compute 10^(digits-1) */
        seq_t pow_rem = pow10(digits - 1);
        cubes[i] = cubes[i] % pow_rem;
    }

    seq_t result = cubes[n - 1];  /* a(n) corresponds to modified cube(n-1) */
    free(cubes);
    return result;
}
