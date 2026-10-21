/*
 * A230711  Values of y such that x^2 + y^2 = 5^n with x and y coprime and 0 <
 *          x < y.
 * https://oeis.org/A230711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the larger component (in absolute value) of (1+2i)^n; the
   recurrence for the real and imaginary parts is
       a_{n+1} = a_n - 2 b_n
       b_{n+1} = 2 a_n + b_n
   with initial a_1 = 1, b_1 = 2.
   a(1) = 2, a(2) = 4, a(3) = 11, a(4) = 24, ... */

#include <stdint.h>   /* for int8_t */

typedef struct {
    seq_t mag;    /* absolute value */
    int8_t sign;  /* +1 or -1; zero magnitude always has sign +1 */
} pair_t;

/* multiply a signed value by 2 (sign unchanged) */
static inline pair_t shrink2(pair_t p)
{
    pair_t r;
    r.mag   = p.mag << 1;
    r.sign  = p.sign;
    return r;
}

/* add two signed numbers represented as pair_t */
static inline pair_t add(pair_t a, pair_t b)
{
    pair_t r;
    if (a.sign == b.sign) {               /* same sign */
        r.mag  = a.mag + b.mag;
        r.sign = a.sign;
        if (r.mag == (seq_t)0)
            r.sign = 1;
    } else {                              /* opposite signs */
        if (a.mag > b.mag) {
            r.mag  = a.mag - b.mag;
            r.sign = a.sign;
        } else if (b.mag > a.mag) {
            r.mag  = b.mag - a.mag;
            r.sign = b.sign;
        } else {                          /* equal magnitude → zero */
            r.mag  = (seq_t)0;
            r.sign = 1;
        }
    }
    return r;
}

/* subtract b from a  (a - b)  */
static inline pair_t sub(pair_t a, pair_t b)
{
    pair_t negb;
    negb.mag  = b.mag;
    negb.sign = -b.sign;                /* sign is flipped */
    return add(a, negb);
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; 0 is undefined */
        return (seq_t)0;

    if (n == 1)            /* a(1) = 2 */
        return (seq_t)2;

    /* initial (real,imag) for n = 1 */
    pair_t a_p = { (seq_t)1, 1 };   /* 1 + i*2 */
    pair_t b_p = { (seq_t)2, 1 };

    /* iterate from n = 2 up to n */
    for (idx_t i = 2; i <= n; ++i) {
        pair_t two_b  = shrink2(b_p);
        pair_t new_a  = sub(a_p, two_b);

        pair_t two_a  = shrink2(a_p);
        pair_t new_b  = add(two_a, b_p);

        a_p = new_a;
        b_p = new_b;
    }

    return (a_p.mag > b_p.mag) ? a_p.mag : b_p.mag;
}
