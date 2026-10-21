/*
 * A094083  Numerators of ratio of sides of n-th triple of rectangles of unit
 *          area sum around a triangle.
 * https://oeis.org/A094083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=1, a(n)=a(n-2)*((n-2)/(n-1))^2  (n≥3). */
/* Returns the numerator of the fraction a(n). */
#include <stdint.h>

/* Greatest‑common divisor for seq_t – works for built‑in and
   arbitrary‑precision integer classes that support %, ==, ++, --. */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;      /* a(1) numerator */
    if (n == (idx_t)2)
        return (seq_t)1;      /* a(2) numerator */

    seq_t num, den;
    if (n & (idx_t)1) {                /* n odd: start from a(1)=1/1 */
        num = (seq_t)1;
        den = (seq_t)1;
        for (idx_t k = (idx_t)3; k <= n; k += (idx_t)2) {
            seq_t mult_num = (seq_t)(k - (idx_t)2) * (seq_t)(k - (idx_t)2);
            seq_t mult_den = (seq_t)(k - (idx_t)1) * (seq_t)(k - (idx_t)1);
            seq_t new_num = num * mult_num;
            seq_t new_den = den * mult_den;
            seq_t g = gcd_seq(new_num, new_den);
            num = new_num / g;
            den = new_den / g;
        }
    } else {                            /* n even: start from a(2)=1/3 */
        num = (seq_t)1;
        den = (seq_t)3;
        for (idx_t k = (idx_t)4; k <= n; k += (idx_t)2) {
            seq_t mult_num = (seq_t)(k - (idx_t)2) * (seq_t)(k - (idx_t)2);
            seq_t mult_den = (seq_t)(k - (idx_t)1) * (seq_t)(k - (idx_t)1);
            seq_t new_num = num * mult_num;
            seq_t new_den = den * mult_den;
            seq_t g = gcd_seq(new_num, new_den);
            num = new_num / g;
            den = new_den / g;
        }
    }
    return num;
}
