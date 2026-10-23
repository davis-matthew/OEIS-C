/*
 * A106848  a(n) = the number of times the last digit of n must be appended to
 *          n to form a number m such that n divides m, or 0 if no such m
 *          exists.
 * https://oeis.org/A106848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the multiplicative order of 10 modulo r =
 *            9*n/gcd(n,last_digit(n)), provided gcd(r,10)=1; otherwise
 *            a(n)=0. For n ending in 0 the value is 1
 * Range:     u8 n <= 28, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   gcd_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd_small.h"

/* Euclidean algorithm for seq_t types                                     */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t d    = nseq % (seq_t)10;

    /* If the last digit is 0, one append already gives a multiple.         */
    if (d == (seq_t)0)
        return (seq_t)1;

    /* r = 9*n/gcd(n,d)                                                    */
    seq_t g = gcd_seq(nseq, d);
    seq_t r = (seq_t)9 * nseq / g;

    /* No solution if 10 and r share a factor (2 or 5).                     */
    seq_t rmod10 = r % (seq_t)10;
    if (gcd_small((int)rmod10, 10) != 1)
        return (seq_t)0;

    /* Find the multiplicative order of 10 modulo r.                       */
    seq_t cur = (seq_t)10 % r;
    seq_t k   = 1;
    while (cur != (seq_t)1) {
        cur = (cur * (seq_t)10) % r;
        k++;
    }
    return k;
}
