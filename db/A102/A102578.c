/*
 * A102578  a(n) = number of elements of order n in simple group Alt(6) =
 *          L_2(9) of order 360.
 * https://oeis.org/A102578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of elements of order n in Alt(6). Derived by
 *            counting cycle type permutations: a(1) = 1, a(2) = 45 (double
 *            transpositions), a(3) = 80 (3‑cycles + two disjoint 3‑cycles),
 *            a(4) = 90 (product of a 4‑cycle and a transposition), a(5) = 144
 *            (5‑cycles), a(n) = 0 for all other n
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   fact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fact.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n=0, return 0 */

    switch (n)
    {
    case 1:
        return 1;

    case 2: {
        /* number of double transpositions: C(6,4)*3 */
        seq_t c = fact(6) / (fact(4) * fact(2)); /* C(6,4) = 15 */
        return c * 3;                           /* 15*3 = 45 */
    }

    case 3: {
        /* 3‑cycles: 6! / (3 · 3!) = 40 */
        seq_t t1 = fact(6) / (3 * fact(3));

        /* two disjoint 3‑cycles:   (C(6,3)/2) · 4 = 40 */
        seq_t t2 = (fact(6) / (fact(3) * fact(3) * 2)) * 4;

        return t1 + t2;         /* 40 + 40 = 80 */
    }

    case 4: {
        /* (4)(2) type: C(6,4) · (4−1)! = 15 · 6 = 90 */
        seq_t c = fact(6) / (fact(4) * fact(2));
        return c * fact(3);
    }

    case 5:
        /* 5‑cycles: 6! / (5 · (6−5)!) = 720 / 5 = 144 */
        return fact(6) / (5 * fact(1));

    default:
        return 0;              /* no elements of other orders */
    }
}
