/*
 * A125889  Denominator of sum of first n ratios of Fibonacci to Lucas
 *          numbers.
 * https://oeis.org/A125889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator( sum_{i=1..n} F(i)/L(i) )
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  F(i) – Fibonacci number, L(i) – Lucas number.  */

#include <stdint.h>

/* Euclidean algorithm for seq_t (works for all integer sizes). */
static seq_t my_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* a(0) = 1   (empty sum) */
    if (n == 0)
        return (seq_t)1;

    seq_t num = (seq_t)0;   /* numerator of the accumulated sum     */
    seq_t den = (seq_t)1;   /* denominator of the accumulated sum   */

    /* Fibonacci numbers iteratively: F_0 = 0, F_1 = 1 */
    seq_t fib_prev = (seq_t)0;  /* F_{i-1}  */
    seq_t fib_curr = (seq_t)1;  /* F_i      */

    for (idx_t i = 1; i <= n; ++i) {
        /* Current term: f = F_i,  l = L_i = F_i + 2*F_{i-1} */
        seq_t f = fib_curr;
        seq_t l = f + fib_prev * (seq_t)2;

        /* Add f/l to current sum: (num*l + den*f) / (den*l) */
        seq_t new_num = num * l + den * f;
        seq_t new_den = den * l;

        /* Reduce the fraction */
        seq_t g = my_gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;

        /* Move to next Fibonacci number: F_{i+1} = F_i + F_{i-1} */
        seq_t next_fib = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = next_fib;
    }

    return den;
}
