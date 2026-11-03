/*
 * A184919  n+[rn/u]+[sn/u]+[tn/u], where []=floor and r=1, s=2^(1/4), t=s^2,
 *          u=s^3.
 * https://oeis.org/A184919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/2^(3/4)⌋ + ⌊n/√2⌋ + ⌊n/2^(1/4)⌋ where ⌊x⌋ denotes
 *            the floor of x. This is equivalent to a(n) = n + ⌊rn/u⌋ + ⌊sn/u⌋
 *            + ⌊tn/u⌋ with r=1, s=2^(1/4), t=s^2, u=s^3. All computations are
 *            performed using integer arithmetic; no floating‑point operations
 *            are employed
 * Range:     u8 n <= 82, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: partial, pass, 120 terms through n = 120 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute k = ⌊n / α⌋ where α^exp = N (N and exp are positive integers).
   The function uses binary search on k (0 ≤ k ≤ n) and compares
   N*k^exp with n^exp, all in 128‑bit arithmetic to avoid overflow. */
static inline idx_t floor_over_alpha(idx_t n, unsigned N, int exp)
{
    if (n == 0)
        return 0;

    idx_t low = 0, high = n;             /* floor can never exceed n */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;

        /* compute N * mid^exp */
        unsigned __int128 lhs = N;
        unsigned __int128 mid_pow = 1;
        for (int i = 0; i < exp; ++i)
            mid_pow *= (unsigned __int128)mid;
        lhs *= mid_pow;

        /* compute n^exp */
        unsigned __int128 rhs = 1;
        for (int i = 0; i < exp; ++i)
            rhs *= (unsigned __int128)n;

        if (lhs <= rhs)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* floor(n/2^(3/4)) : exponent 4, N = 8 */
    idx_t f_3_quarter = floor_over_alpha(n, 8, 4);
    /* floor(n/√2) : exponent 2, N = 2 */
    idx_t f_sqrt2 = floor_over_alpha(n, 2, 2);
    /* floor(n/2^(1/4)) : exponent 4, N = 2 */
    idx_t f_1_quarter = floor_over_alpha(n, 2, 4);

    seq_t sum = (seq_t)n
              + (seq_t)f_3_quarter
              + (seq_t)f_sqrt2
              + (seq_t)f_1_quarter;
    return sum;
}
