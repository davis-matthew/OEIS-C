/*
 * A032520  Sum of the integer part of 13/3-th roots of integers less than n.
 * https://oeis.org/A032520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} ⌊k^(3/13)⌋
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_int.h"

/* Utility: Return true if base^k <= x without overflow. */
static bool pow_leq(seq_t base, idx_t k, seq_t x)
{
    seq_t res = 1;
    for (idx_t i = 0; i < k; ++i) {
        if (base == 0)
            return true;          /* 0^k == 0 <= x  */
        if (res > x / base)        /* next multiplication would overflow or exceed x */
            return false;
        res *= base;
    }
    return true;
}

/* Integer kth root: largest r such that r^k <= x.  */
static seq_t int_kth_root(seq_t x, idx_t k)
{
    seq_t lo = 0;
    seq_t hi = 1;

    /* Find an upper bound for the root. */
    while (pow_leq(hi, k, x)) {
        if (hi >= x) {
            hi = x + 1;   /* the root cannot exceed x */
            break;
        }
        hi <<= 1;
    }

    /* Binary search between lo and hi. */
    while (lo + 1 < hi) {
        seq_t mid = lo + (hi - lo) / 2;
        if (pow_leq(mid, k, x))
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* Main function for sequence A032520. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t t = (seq_t)i;
        seq_t cube = t * t * t;
        seq_t r = int_kth_root(cube, 13);
        sum += r;
    }
    return sum;
}
