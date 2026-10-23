/*
 * A145887  Number of excedances in all even permutations of {1,2,...,n} with
 *          no fixed points.
 * https://oeis.org/A145887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n^2-3*n-2)*a(n-1)+(n-1)*(n+1)*a(n-2))/(n-3) for n>=4, *
 *            with a(1)=0, a(2)=0, a(3)=3
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the nth term of OEIS sequence A145887. */
seq_t a(idx_t n)
{
    /* Base cases. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)3;

    /* a(i-2) and a(i-1) for the current iteration. */
    seq_t a_prev2 = (seq_t)0;   /* a(2) */
    seq_t a_prev1 = (seq_t)3;   /* a(3) */
    seq_t a_curr   = (seq_t)0;  /* will hold a(i) */

    for (idx_t i = 4; i <= n; ++i)
    {
        if (sizeof(seq_t) <= 8)
        {
            /* Use 128‑bit integers for intermediate results when seq_t
               is 64‑bit or smaller. */
            __int128 ii      = (__int128)i;
            __int128 coeff1  = ii*ii - 3*ii - 2;          /* i^2-3i-2 */
            __int128 coeff2  = (ii-1)*(ii+1);             /* (i-1)(i+1) */
            __int128 numer   = coeff1 * (__int128)a_prev1
                            + coeff2 * (__int128)a_prev2;
            __int128 denom   = ii-3;
            a_curr = (seq_t)(numer / denom);
        }
        else
        {
            /* For arbitrary‑precision seq_t use normal arithmetic. */
            seq_t ii        = (seq_t)i;
            seq_t coeff1    = ii*ii - (seq_t)3*ii - (seq_t)2;
            seq_t coeff2    = (ii-1)*(ii+1);
            seq_t numer     = coeff1 * a_prev1;
            numer           = numer + coeff2 * a_prev2;
            seq_t denom     = ii - (seq_t)3;
            a_curr          = numer / denom;
        }

        /* Shift for next iteration. */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;  /* a(n) */
}
