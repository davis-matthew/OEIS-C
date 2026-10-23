/*
 * A011937  a(n) = floor( n*(n-1)*(n-2)*(n-3)/27 ).
 * https://oeis.org/A011937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)*(n-2)*(n-3)/27)
 * Range:     u8 n <= 10, u16 n <= 37, u32 n <= 585, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*
 * Computes the n-th term of OEIS sequence A011937.
 *
 * Sequence: a(n) = ⌊ n*(n-1)*(n-2)*(n-3) / 27 ⌋
 * Offset: 0  (a(0) = 0)
 *
 * The function handles any n >= 0, using a 128‑bit intermediate
 * to avoid overflow on the product of the four factors.  For
 * small n (n<4) the product is zero, so the result is 0.
 */
seq_t a(idx_t n)
{
    /* For n < 4 the product contains a zero factor → result 0. */
    if (n < 4)
        return 0;

    /* Compute the product using a 128‑bit unsigned integer */
    __uint128_t prod = (__uint128_t)n * (n - 1) * (n - 2) * (n - 3);
    prod /= 27;                     /* floor division: same as integer division */

    return (seq_t)prod;
}
