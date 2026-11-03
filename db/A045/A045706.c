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
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

# include <stdint.h>
# include <stdbool.h>
# include <stddef.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* binary search to test if a value is in the fibonacci list */
static inline bool is_fib_value(seq_t value, seq_t *fib, int count)
{
    int lo = 0, hi = count - 1;
    while (lo <= hi)
    {
        int mid = (lo + hi) >> 1;
        if (fib[mid] == value) return true;
        if (fib[mid] < value)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return false;
}

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
