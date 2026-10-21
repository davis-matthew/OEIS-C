/*
 * A074004  Number of elements of GF(3^n) with trace 1 and subtrace 1.
 * https://oeis.org/A074004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   * a(n) = (3^n + s(n) * 3^ceil(n/2)) / 9 * * Where s(n) is
 *            defined from k = floor(n/2) as follows: * if n is even: * k mod
 *            6 == 2 → s = 3 * k mod 6 == 5 → s = -3 * otherwise → s = 0 * if
 *            n is odd: * k mod 6 == 0 → s = -1 * k mod 6 == 1 → s = 0 * k mod
 *            6 == 2 → s = -2 * k mod 6 == 3 → s = 1 * k mod...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   mul_small, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Compute 3^n and 3^ceil(n/2) */
    seq_t pow3n = seq_pow((seq_t)3, n);
    idx_t ceil_n2 = (n + 1) >> 1;            /* ceil(n/2) */
    seq_t pow3c  = seq_pow((seq_t)3, ceil_n2);

    /* Determine coefficient s(n) */
    idx_t k = n >> 1;        /* floor(n/2) */
    int rem = (int)(k % 6U); /* 0..5 */

    int s = 0;               /* coefficient to multiply pow3c */
    if (n & 1U) {            /* n odd */
        switch (rem) {
            case 0: s = -1; break;
            case 1: s =  0; break;
            case 2: s = -2; break;
            case 3: s =  1; break;
            case 4: s =  0; break;
            case 5: s =  2; break;
        }
    } else {                 /* n even */
        switch (rem) {
            case 2: s =  3; break;
            case 5: s = -3; break;
            default: s = 0; break;
        }
    }

    /* Numerator: pow3n + s * pow3c (s may be negative) */
    seq_t numerator = pow3n;
    if (s >= 0) {
        numerator += mul_small(pow3c, (unsigned int)s);
    } else {
        numerator -= mul_small(pow3c, (unsigned int)(-s));
    }

    /* Final division by 9 */
    return numerator / (seq_t)9;
}
