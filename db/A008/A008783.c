/*
 * A008783  Period of continued fraction representation of
 *          (sqrt(4n+1)+1)/2=sqrt(n+sqrt(n+sqrt(n+...))).
 * https://oeis.org/A008783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Validated: partial, pass, 93 terms through n = 93 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Period of the continued fraction of (sqrt(4n+1)+1)/2 (A008783) */

#include <stddef.h>
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

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t D = (seq_t)4 * n + 1;
    seq_t s = floor_sqrt(D);
    if (s * s == D) return 0;        /* the number is rational */

    seq_t P = 1;                     /* initial coefficients */
    seq_t Q = 2;

    /* Period length is bounded by ~2*sqrt(D). 4096 is safely large. */
    const size_t MAX_STATES = 4096;
    seq_t P_arr[MAX_STATES];
    seq_t Q_arr[MAX_STATES];
    size_t count = 0;

    P_arr[count] = P;
    Q_arr[count] = Q;
    ++count;

    while (1)
    {
        seq_t a_term = (P + s) / Q;     /* floor((P+sqrt(D))/Q) */
        seq_t P1 = a_term * Q - P;
        seq_t Q1 = (D - P1 * P1) / Q;

        /* look for a repeated state */
        size_t i;
        for (i = 0; i < count; ++i)
        {
            if (P_arr[i] == P1 && Q_arr[i] == Q1)
                break;
        }

        if (i < count)
        {
            /* period = number of distinct states until repetition */
            return (seq_t)(count - i);
        }

        if (count >= MAX_STATES - 1) return 0;  /* safety fallback */

        P_arr[count] = P1;
        Q_arr[count] = Q1;
        ++count;

        P = P1;
        Q = Q1;
    }
}
