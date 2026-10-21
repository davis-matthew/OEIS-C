/*
 * A230842  Values of y such that x^2 + y^2 = 61^n with x and y coprime and 0
 *          < x < y.
 * https://oeis.org/A230842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A230842: y-values for x^2 + y^2 = 61^n with 0 < x < y and gcd(x,y)=1.
   The sequence is obtained from the complex power (5+6i)^n.
   Since 5^2 + 6^2 = 61, each power (5+6i)^n = a_n + i*b_n
   satisfies the recurrence
      a_{k+1} = 5*a_k - 6*b_k
      b_{k+1} = 6*a_k + 5*b_k
   with a_0 = 1, b_0 = 0.
   The desired y is max(|a_n|, |b_n|).
*/

#include <stdbool.h>
#include "../utils/seq_mul.h"

/* Signed integer with unsigned magnitude */
typedef struct {
    bool neg;   /* true if negative, false otherwise */
    seq_t mag;  /* absolute value, unsigned */
} signed_num;

/* Addition of two signed numbers */
static signed_num s_add(const signed_num *x, const signed_num *y)
{
    signed_num r;
    if (x->neg == y->neg) {          /* same sign -> add magnitudes */
        r.neg = x->neg;
        r.mag = x->mag + y->mag;
    } else {                         /* opposite signs -> subtract smaller magnitude */
        if (x->mag >= y->mag) {
            r.neg = x->neg;
            r.mag = x->mag - y->mag;
        } else {
            r.neg = y->neg;
            r.mag = y->mag - x->mag;
        }
    }
    return r;
}

/* Subtraction x - y */
static signed_num s_sub(const signed_num *x, const signed_num *y)
{
    /* Subtraction is addition of x and (-y) */
    signed_num ny = *y;
    ny.neg = !ny.neg;
    return s_add(x, &ny);
}

/* Multiplication by a small unsigned integer m */
static signed_num s_mul_small(const signed_num *x, unsigned int m)
{
    signed_num r;
    r.neg = x->neg;
    r.mag = seq_mul(x->mag, (seq_t)m);
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* out of offset, but safe return */

    signed_num a = {false, (seq_t)1}; /* a_0 = 1 */
    signed_num b = {false, (seq_t)0}; /* b_0 = 0 */

    for (idx_t i = 0; i < n; ++i) {
        /* Compute new_a = 5*a - 6*b */
        signed_num five_a = s_mul_small(&a, 5);
        signed_num six_b = s_mul_small(&b, 6);
        signed_num new_a = s_sub(&five_a, &six_b);

        /* Compute new_b = 6*a + 5*b */
        signed_num six_a = s_mul_small(&a, 6);
        signed_num five_b = s_mul_small(&b, 5);
        signed_num new_b = s_add(&six_a, &five_b);

        a = new_a;
        b = new_b;
    }

    /* y is the larger magnitude of a and b */
    return (a.mag > b.mag) ? a.mag : b.mag;
}
