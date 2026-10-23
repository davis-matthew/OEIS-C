/*
 * A166050  a(n) = Sum_{i=0..(2n+1)} J(i,12n+7), where J(i,k) is the Jacobi
 *          symbol.
 * https://oeis.org/A166050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{i=0}^{2n+1} J(i,12n+7) where J is the Jacobi symbol
 * Range:     i8 n <= 1641, i16 n <= 9951, i32 n <= 9951, i64 n <= 9951,
 *            i128 n <= 9951, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Jacobi symbol (a|n) for odd n > 0.
   Returns -1, 0, or 1. */
static int jacobi_symbol(uint64_t a, uint64_t n)
{
    if (n == 0) return 0;          /* not defined, but unused */
    a %= n;
    if (a == 0) return 0;
    int result = 1;

    while (a != 0) {
        /* Factor out powers of 2 from a. */
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t r = n & 7;    /* n mod 8 */
            if (r == 3 || r == 5)
                result = -result;
        }

        /* Swap a and n (quadratic reciprocity). */
        uint64_t temp = a;
        a = n;
        n = temp;

        if ((a & 3) == 3 && (n & 3) == 3)
            result = -result;

        a %= n;
    }
    return (n == 1) ? result : 0;
}

/* Compute the nth term of OEIS sequence A166050. */
seq_t a(idx_t n)
{
    /* The modulus 12n+7 is positive and odd. */
    uint64_t k = 12u * n + 7u;

    seq_t sum = 0;
    idx_t limit = 2 * n + 1;        /* upper bound, inclusive */
    for (idx_t i = 0; i <= limit; ++i) {
        sum += (seq_t)jacobi_symbol(i, k);
    }
    return sum;
}
