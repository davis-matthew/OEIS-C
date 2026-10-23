/*
 * A104054  Numbers which are the sum of three positive cubes and divisible by
 *          31.
 * https://oeis.org/A104054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer that is divisible by 31 and
 *            representable as a sum of three positive integer cubes
 * Range:     u8 n <= 2, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   cube, cube_root_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External type definitions */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utilities provided by the build */
#include "../utils/cube.h"
#include "../utils/cube_root_floor.h"

/* Helper to compute the integer ceil of the cube root */
static inline seq_t ceil_cube_root_floor(seq_t x)
{
    seq_t r = cube_root_floor(x);
    return (cube(r) == x) ? r : r + 1;
}

/* Test whether x can be written as sum of three positive cubes. */
static bool is_sum_of_three_cubes(seq_t x)
{
    /* Must be > 0 */
    if (x == 0) return false;

    /* Upper bound for cube roots */
    idx_t root = (idx_t)ceil_cube_root_floor(x);   /* safe: root small */

    for (idx_t a = 1; a <= root; ++a)
    {
        seq_t a3 = cube((seq_t)a);
        if (a3 >= x) break;                /* further a's too large */

        for (idx_t b = 1; b <= root; ++b)
        {
            seq_t ab = a3 + cube((seq_t)b);
            if (ab >= x) break;            /* further b's too large */

            seq_t c3 = x - ab;
            if (c3 == 0) return true;      /* a^3 + b^3 = x, c = 0 impossible
                                               (c must be positive) */

            seq_t c = cube_root_floor(c3);
            if (cube(c) == c3)              /* c positive automatically */
                return true;
        }
    }
    return false;
}

/* Public function: return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* irrelevant for < offset */

    seq_t found = 0;                          /* already found terms count */
    idx_t k = 1;                               /* multiplier for 31 */

    for (;; ++k)
    {
        seq_t cand = (seq_t)31 * (seq_t)k;     /* candidate divisible by 31 */

        if (is_sum_of_three_cubes(cand))
        {
            ++found;
            if (found == n)
                return cand;
        }
    }
}
