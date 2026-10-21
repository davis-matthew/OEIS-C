/*
 * A230339  Numerator of Sum_{k=1..n} 1/(k(k+1)(k+2)(k+3)) = Sum_{k=1..n}
 *          1/Pochhammer(k,4).
 * https://oeis.org/A230339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator(1/18 - 1/(3*(n+1)*(n+2)*(n+3)))
 * Range:     u8 n <= 0, u16 n <= 13, u32 n <= 732, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = (P-6)/gcd(P-6,18*P) where P = (n+1)(n+2)(n+3) */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t n1 = (seq_t)n + 1;
    seq_t n2 = (seq_t)n + 2;
    seq_t n3 = (seq_t)n + 3;
    seq_t P   = n1 * n2 * n3;         /* (n+1)(n+2)(n+3)                  */
    seq_t num = P - 6;                /* P - 6                            */
    seq_t den = 18 * P;               /* 18 * P                           */
    seq_t g   = gcd(num, den);        /* greatest common divisor           */
    return num / g;                   /* reduced numerator (a(n))          */
}
