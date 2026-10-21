/*
 * A085571  Numerator of 2*Sum(C(n,w)/w,w=1..n/2-1)+C(n,n/2)/(n/2) if n is
 *          even otherwise of 2*Sum(C(n,w)/w,w=1..(n-1)/2).
 * https://oeis.org/A085571
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of 2*sum_{w=1}^{⌊(n-1)/2⌋} C(n,w)/w + (if n
 *            even) C(n,n/2)/(n/2)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
 
/* Euclidean GCD for seq_t.  It is local but not named seq_gcd. */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}
 
/* Compute the numerator of the rational expression defined above. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* values for n < 2 are irrelevant */
 
    /* Number of terms in the main sum */
    idx_t m = (n - 1) >> 1;       /* floor((n-1)/2) */
    /* Half of n, used if n is even */
    idx_t half = n >> 1;          /* n/2 */
 
    seq_t num = 0;                /* numerator of accumulated fraction */
    seq_t den = 1;                /* denominator of accumulated fraction */
 
    seq_t c = 1;                  /* running binomial coefficient C(n,0) */
 
    /* 2 * Sum_{w=1}^{m} C(n,w)/w */
    for (idx_t w = 1; w <= m; ++w) {
        /* Update C(n,w) from C(n,w-1) */
        c = (c * (seq_t)(n - w + 1)) / (seq_t)w;
 
        seq_t tnum = c * (seq_t)2;   /* 2*C(n,w) */
        seq_t tden = (seq_t)w;       /* w   */
 
        /* new_num/new_den = num/den + tnum/tden */
        seq_t new_num = num * tden + tnum * den;
        seq_t new_den = den * tden;
 
        seq_t g = gcd_seq(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }
 
    /* If n is even, add C(n, n/2)/(n/2) */
    if ((n & 1) == 0) {
        /* Compute C(n,half) from C(n,half-1) */
        c = (c * (seq_t)(n - half + 1)) / (seq_t)half;
 
        seq_t tnum = c;                 /* C(n,half) */
        seq_t tden = (seq_t)half;       /* half   */
 
        seq_t new_num = num * tden + tnum * den;
        seq_t new_den = den * tden;
 
        seq_t g = gcd_seq(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }
 
    /* The fraction has been reduced; the numerator is the desired value */
    return num;
}
