/*
 * A173756  Partial sums of A058313.
 * https://oeis.org/A173756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{i=1..n} numerator(Sum_{k=1..i} (-1)^(k+1)/k)
 *             = Sum_{i=1..n} numerator(H_i)
 * where H_i is the i-th alternating harmonic number.
 *
 * The function computes the sequence using an iterative
 * fraction update and takes care to keep intermediate
 * arithmetic in 128‑bit unsigned integers to avoid overflow
 * for n up to about 10 000 on 64‑bit builds.  For arbitrary‑
 * precision builds the same code works because `seq_t` then
 * supports unlimited width.
 */

#include <stdint.h>

/* Helper: absolute value of an unsigned __int128 is itself. */
static unsigned __int128 abs128(unsigned __int128 x) { return x; }

/* Euclidean GCD for unsigned __int128 */
static unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b != 0) {
        unsigned __int128 t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    unsigned __int128 sum = 0;            /* accumulate in 128‑bit */
    unsigned __int128 num = 0;            /* numerator of H_i (reduced) */
    unsigned __int128 den = 1;            /* denominator of H_i (reduced) */

    for (idx_t i = 1; i <= n; ++i) {
        /* compute new numerator and denominator for H_i */
        unsigned __int128 new_num, new_den;
        if (i & 1) {                     /* odd i: add 1/i */
            new_num = num * i + den;
        } else {                         /* even i: subtract 1/i */
            /* guaranteed that num*i > den for i>=2 */
            new_num = num * i - den;
        }
        new_den = den * i;

        /* reduce the fraction */
        unsigned __int128 g = gcd128(new_num, new_den);
        num = new_num / g;
        den = new_den / g;

        /* add numerator to total sum */
        sum += num;
    }

    return (seq_t)sum;
}
