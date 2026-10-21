/*
 * A228704  Number of 4-irreducible maps made up of two hexagons and n
 *          squares.
 * https://oeis.org/A228704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=6, a(2)=21, a(3)=62, a(4)=180
   for n ≥ 5:
        n*(9*n-29)*(3*n-10)*a(n)
     - 2*(9*n-20)*(2*n-7)*(3*n-7)*a(n-1) = 0
   (equivalently a(n) = [2*(9*n-20)*(2*n-7)*(3*n-7) / (n*(9*n-29)*(3*n-10))] * a(n-1)
   with arbitrary‑precision arithmetic; the production of the product and the
   division are performed in a way that avoids overflow. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Base values for n = 0 … 4 */
    static const seq_t base[] = {1, 6, 21, 62, 180};

    if (n < 5)
        return base[n];

    seq_t prev = base[4];
    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t si = (seq_t)i;

        /* Compute the numerator and denominator of the factor a(n)/a(n-1) */
        seq_t num = 2 * (9 * si - 20) * (2 * si - 7) * (3 * si - 7);
        seq_t den = si * (9 * si - 29) * (3 * si - 10);

        /* Cancel common factors with prev to keep intermediate sizes small */
        seq_t g1 = gcd(prev, den);
        seq_t prev1 = prev / g1;
        seq_t den1  = den  / g1;

        seq_t g2 = gcd(num, den1);
        seq_t num1 = num / g2;
        seq_t den2 = den1 / g2;

        /* Compute prev1 * num1 / den2 safely using 128‑bit intermediate */
        __int128 prod = (__int128)prev1 * ( (__int128)num1 );
        prev = (seq_t)(prod / den2);
    }

    return prev;
}
