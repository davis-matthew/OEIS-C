/*
 * A033784  Product t2(q^d); d | 25, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(a,b,c)∈Z≥0^3 | a(a+1) + 5·b(b+1) + 25·c(c+1) = 2 · n}
 * Range:     u8 n <= 7, u16 n <= 2032, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9809 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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

/* Does x equal a·(a+1) for some integer a≥0? */
static bool is_triangular(seq_t x)
{
    /* D = 4·x + 1 must be a perfect square of an odd integer */
    seq_t D = (seq_t)1 + (seq_t)4 * x;
    seq_t s = floor_sqrt(D);
    return s * s == D;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)2 * (seq_t)n;   /* 2·n */
    seq_t count  = (seq_t)0;

    for (idx_t C = 0; ; ++C) {
        seq_t c_term = (seq_t)C * ((seq_t)C + (seq_t)1);      /* c(c+1) */
        seq_t costC  = (seq_t)25 * c_term;                     /* 25·c(c+1) */
        if (costC > target)
            break;
        seq_t remC = target - costC;

        for (idx_t B = 0; ; ++B) {
            seq_t b_term = (seq_t)B * ((seq_t)B + (seq_t)1);   /* b(b+1) */
            seq_t costB  = (seq_t)5 * b_term;                  /* 5·b(b+1) */
            if (costB > remC)
                break;
            seq_t remB = remC - costB;

            if (is_triangular(remB))
                ++count;
        }
    }

    return count;
}
