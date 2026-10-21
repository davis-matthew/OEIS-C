/*
 * A066545  Number of spanning trees in the line graph of the product of two
 *          complete graph, each of order n, L(K_n x K_n).
 * https://oeis.org/A066545
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  hard, nonn
 * Formula:   * a(n) = ((4*n-4)^(n^2*(n-2)) * n^(n^2-3) * 2^((n-1)^2)) / (n-1)
 *            * * Computes the number of spanning trees in the line graph of
 *            the * Cartesian product of two complete graphs K_n × K_n
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;            // value not defined for n < 2

    /* Convert n to seq_t for arithmetic with the arbitrary‑precision type */
    seq_t m = (seq_t)n;

    /* (4*n - 4)^(n^2*(n-2)) */
    seq_t base1 = 4 * m - 4;
    seq_t exp1  = m * m * (m - 2);          /* n^2 * (n-2) */
    seq_t t1    = big_pow(base1, exp1);

    /* 2^((n-1)^2) */
    seq_t exp2  = (m - 1) * (m - 1);       /* (n-1)^2 */
    seq_t t2    = big_pow((seq_t)2, exp2);

    /* n^(n^2-3) */
    seq_t exp3  = m * m - (seq_t)3;        /* n^2 - 3 */
    seq_t t3    = big_pow(m, exp3);

    /* Combine the factors and divide by (n-1) */
    seq_t result = t1;
    result *= t2;
    result *= t3;
    result /= (m - (seq_t)1);              /* exact division */

    return result;
}
