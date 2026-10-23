/*
 * A190987  a(n) = 10*a(n-1) - 5*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 5*a(n-2), a(0) = 0, a(1) = 1 Closed form:
 *            a(2n) = F(6n) * 5^n / 4 a(2n+1) = L(6n+3) * 5^n / 4 where F is
 *            Fibonacci, L is Lucas
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   fib, fib_pair, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/fib_pair.h"
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* even indices */
    if ((n & 1ULL) == 0) {
        idx_t m = n >> 1;                /* m = n/2                     */
        seq_t f   = fib(6 * m);          /* F(6m)                        */
        seq_t p5  = power5(m);           /* 5^m                          */
        seq_t res = f * p5;              /* F(6m)*5^m                    */
        return res / (seq_t)4;           /* divide by 4                   */
    }

    /* odd indices */
    {
        idx_t m = (n - 1) >> 1;          /* m = (n-1)/2                  */
        idx_t k = 6 * m + 3;             /* k = 6m + 3                    */
        seq_t fk, fk1;                   /* F(k), F(k+1) via fast doubling */
        fib_pair(k, &fk, &fk1);
        seq_t Lk  = (fk1 * (seq_t)2) - fk; /* L(k) = 2*F(k+1) - F(k)      */
        seq_t p5  = power5(m);           /* 5^m                          */
        seq_t res = Lk * p5;             /* L(k)*5^m                      */
        return res / (seq_t)4;           /* divide by 4                   */
    }
}
