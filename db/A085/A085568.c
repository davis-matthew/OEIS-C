/*
 * A085568  Numerator of 2*Sum(C(n,w)/(2*w+1),w=0..n/2-1)+C(n,n/2)/(n+1) if n
 *          is even, or of 2*Sum(C(n,w)/(2*w+1),w=0..(n-1)/2) if n is odd.
 * https://oeis.org/A085568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of 2*sum_{w=0}^{⌊(n-1)/2⌋} C(n,w)/(2*w+1) + (n
 *            even? C(n,n/2)/(n+1) : 0)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binom_at, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/binom_at.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;

    seq_t P = 0;          /* current numerator  */
    seq_t Q = 1;          /* current denominator */

    idx_t maxw;
    if (n & 1)               /* n odd */
        maxw = (n - 1) / 2;
    else                     /* n even */
        maxw = n / 2 - 1;

    seq_t bin = 1;          /* C(n,0) */

    for (idx_t w = 0; w <= maxw; ++w) {
        seq_t t_num = bin * ((seq_t)2);            /* 2 * C(n,w) */
        seq_t t_den = (seq_t)(2 * w + 1);

        seq_t newP = P * t_den + t_num * Q;
        seq_t newQ = Q * t_den;
        seq_t g    = gcd(newP, newQ);

        P = newP / g;
        Q = newQ / g;

        /* update binomial for next w */
        if (w != maxw) {
            bin = bin * (n - w);
            bin = bin / (w + 1);
        }
    }

    if ((n & 1) == 0) {   /* even additional term */
        seq_t bin_mid = binom_at(n, n / 2);
        seq_t t_num   = bin_mid;
        seq_t t_den   = (seq_t)(n + 1);

        seq_t newP = P * t_den + t_num * Q;
        seq_t newQ = Q * t_den;
        seq_t g    = gcd(newP, newQ);

        P = newP / g;
        Q = newQ / g;
    }

    return P;   /* numerator of the result */
}
