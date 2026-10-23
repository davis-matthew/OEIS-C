/*
 * A030697  Smallest nontrivial extension of n-th cube which is a cube not
 *          ending 000.
 * https://oeis.org/A030697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 100, u128 n <= 10000,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A030697
   Definition:
   a(n) = the smallest integer (cube) M > n³ that starts with the digits of n³
   and does not end in 000.

   Algorithm:
   For each n we compute P = n³.
   We then increase the number of appended digits k (k ≥ 1):
       low  = P * 10ᵏ
       high = (P+1) * 10ᵏ – 1
   All cubes in this interval are candidates.
   The integer cube roots of low and high bound the search:
       t_min = ceil(cuberoot(low))
       t_max = floor(cuberoot(high))
   For every integer t in [t_min, t_max] we compute M = t³.
   If M does not end with 000, has more digits than P
   and the leading digits of M equal P, we have found the answer.
   The search is guaranteed to terminate quickly (k never exceeds 50
   for n ≤ 10 000).  All arithmetic is done with the arbitrary‑precision
   type seq_t together with helper functions for digit counting and
   cube‑root bounds.  */
 
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"

/* digit length of a seq_t value (works for arbitrary precision) */
static int digit_len(seq_t x)
{
    int len = 0;
    do { ++len; x /= 10; } while (x);
    return len;
}

/* floor cube root of a seq_t value, result fits in uint64_t */
static uint64_t floor_cuberoot(seq_t x)
{
    if (x == (seq_t)0) return 0;
    uint64_t lo = 0, hi = 1;
    seq_t hi3 = (seq_t)hi * hi * hi;
    while (hi3 <= x) {
        hi <<= 1;
        hi3 = (seq_t)hi * hi * hi;
    }
    while (lo + 1 < hi) {
        uint64_t mid = (lo + hi) >> 1;
        seq_t mid3 = (seq_t)mid * mid * mid;
        if (mid3 <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* ceil cube root of a seq_t value, result fits in uint64_t */
static uint64_t ceil_cuberoot(seq_t x)
{
    if (x == (seq_t)0) return 0;
    uint64_t f = floor_cuberoot(x);
    seq_t f3 = (seq_t)f * f * f;
    if (f3 == x)
        return f;
    return f + 1;
}

seq_t a(idx_t n)
{
    /* P = n³ */
    seq_t P = (seq_t)n * (seq_t)n * (seq_t)n;
    int L = digit_len(P);               /* length of the base cube */

    seq_t pow10k = 1;                    /* 10ᵏ, starts with k=0 */

    for (unsigned k = 1; k <= 50; ++k) {
        pow10k *= (seq_t)10;               /* update to 10ᵏ */

        seq_t low  = P * pow10k;
        seq_t high = ((P + (seq_t)1) * pow10k) - (seq_t)1;

        uint64_t t_min = ceil_cuberoot(low);
        uint64_t t_max = floor_cuberoot(high);
        if (t_min > t_max) continue;      /* no cube in this interval */

        for (uint64_t t = t_min; t <= t_max; ++t) {
            seq_t m = (seq_t)t * (seq_t)t * (seq_t)t;   /* candidate cube */

            if ((m % (seq_t)1000) == 0) continue;      /* exclude trailing 000 */

            int diff = digit_len(m) - L;
            if (diff <= 0) continue;                   /* must be longer */

            seq_t divisor = seq_pow((seq_t)10, (idx_t)diff);
            if ((m / divisor) == P) return m;          /* prefix matches */
        }
    }

    /* Should never happen for the expected range of n. */
    return (seq_t)0;
}
