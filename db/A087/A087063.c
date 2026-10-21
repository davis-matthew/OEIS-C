/*
 * A087063  Sum{Floor(n*(r^k)): r=sqrt(2)-1, k>=0}.
 * https://oeis.org/A087063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k ≥ 0} floor( n * (sqrt(2)-1)^k ), a(0) = 0
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   floor_sqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt_u128.h"

seq_t a(idx_t n)
{
    /* Handle the offset directly. */
    if (n == 0)
        return 0;

    /* k = 0 term is simply n. */
    seq_t sum = (seq_t)n;

    /* Recurrence for coefficients of r^k = (sqrt(2)-1)^k = A_k*sqrt(2) + B_k.
       Start with k = 1 : A = 1, B = -1. */
    long long A = 1;
    long long B = -1;

    for (;;)
    {
        /* Compute floor( n * (A*sqrt(2) + B) ). */
        __int128 m = (__int128)n * (__int128)A;          /* m = n*A */
        unsigned __int128 abs_m = m >= 0 ? (unsigned __int128)m
                                        : (unsigned __int128)(-m);
        unsigned __int128 t = abs_m * abs_m * 2u;        /* 2*m*m   */
        unsigned __int128 s = floor_sqrt_u128(t);        /* floor(sqrt(t)) = floor(|m|*sqrt(2)) */

        __int128 floor_val;
        if (m >= 0)
            floor_val = (__int128)s;
        else
            floor_val = -((__int128)s) - 1;

        __int128 term128 = (__int128)B * (__int128)n + floor_val;

        /* If the term becomes zero (or negative by cancellation) we are done. */
        if (term128 <= 0)
            break;

        sum += (seq_t)term128;

        /* Update coefficients to get r^(k+1). */
        long long nextA = -A + B;      /* A_{k+1} = -A_k + B_k */
        long long nextB = 2 * A - B;   /* B_{k+1} = 2*A_k - B_k */
        A = nextA;
        B = nextB;
    }

    return sum;
}
