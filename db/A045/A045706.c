/*
 * A045706  Number of ways n can be written as a sum of a square of a
 *          Fibonacci number and a cube of a Fibonacci number; F(1) = F(2) = 1
 *          are considered the same.
 * https://oeis.org/A045706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations n = F^2 + G^3 where F and G are
 *            * distinct Fibonacci numbers (0,1,2,3,5,8,13,21,34,55,89,…)
 *            counted once per * value. Both orderings (square coming from F
 *            or from G) are distinct
 * Range:     u8 n <= 63, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   floor_sqrt, is_fib_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

# include <stdint.h>
# include <stdbool.h>
# include <stddef.h>
# include "../utils/floor_sqrt.h"
#include "../utils/is_fib_value.h"

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* 1. Determine the upper bound for Fibonacci values we must generate.   */
    seq_t max_needed = floor_sqrt(n);   /* sqrt(n) >= cube_root(n) for n>=0 */

    /* 2. Generate the distinct Fibonacci numbers up to max_needed.          */
    seq_t fib[200];
    int fib_cnt = 0;
    seq_t f0 = 0, f1 = 1;
    while (f0 <= max_needed)
    {
        /* avoid duplicate 1 coming from F(1)=F(2)=1 */
        if (fib_cnt == 0 || f0 != fib[fib_cnt - 1])
            fib[fib_cnt++] = f0;
        seq_t f2 = f0 + f1;   /* f0 = previous, f1 = current */
        f0 = f1;
        f1 = f2;
    }

    /* 3. Count all (F^2 + G^3) representations of n.                      */
    seq_t count = 0;
    for (int i = 0; i < fib_cnt; ++i)
    {
        seq_t f = fib[i];
        /* compute f^3 safely – it is guaranteed that f^3 <= n for relevant f */
        seq_t f2 = f * f;     /* f^2 <= n, fits in seq_t because f <= sqrt(n) */
        seq_t cube = f2 * f;  /* f^3 <= n as long as f <= cube_root(n) */
        if (cube > n) break;  /* further f will only produce larger cubes */

        seq_t rem = n - cube;
        seq_t sq = floor_sqrt(rem);
        if (sq * sq == rem && is_fib_value(sq, fib, fib_cnt))
            ++count;
    }
    return count;
}
