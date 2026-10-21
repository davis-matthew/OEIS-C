/*
 * A208743  Number of subsets of the set {1,2,...,n} which do not contain two
 *          elements whose difference is 6.
 * https://oeis.org/A208743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(floor(n/6) + 3)^(n mod 6) * F(floor(n/6) + 2)^(6 - (n
 *            mod 6)) (F(k) = Fibonacci number, F(1)=1, F(2)=1)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the Fibonacci helper */
#include "../utils/fib.h"

/* Small power helper – exponent <= 6 */
static inline seq_t pow_small(seq_t base, unsigned int exp)
{
    seq_t result = 1;
    while (exp--)
        result *= base;
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure n >= 1; behaviour for n < 1 is undefined */
    idx_t q = n / 6;          /* floor(n/6)  */
    idx_t r = n % 6;          /* n mod 6     */

    seq_t f1 = fib(q + 3);    /* F(floor(n/6)+3) */
    seq_t f2 = fib(q + 2);    /* F(floor(n/6)+2) */

    seq_t pow1 = pow_small(f1, (unsigned int)r);
    seq_t pow2 = pow_small(f2, (unsigned int)(6 - r));

    return pow1 * pow2;
}
