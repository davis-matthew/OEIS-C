/*
 * A034801  Triangle of Fibonomial coefficients (k=2).
 * https://oeis.org/A034801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(n,k) = Π_{j=0..k-1} Fib(2*(n-j)) /
 *            Π_{j=1..k} Fib(2*j) (Fib = Fibonacci, 0-indexed). The flat index
 *            n is mapped to row r and column k by traversing the triangular
 *            array. The output is the Fibonomial coefficient at that position
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 58, u64 n <= 110,
 *            u128 n <= 217, big any n
 * Helpers:   fib, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"   /* provides the helper gcd(seq_t, seq_t) */
#include "../utils/fib.h"   /* provides the helper fib(idx_t) */

seq_t a(idx_t n)
{
    /* Determine row r and column k from flat index n */
    idx_t r = 0;
    idx_t cum = 0;
    while (1) {
        /* number of entries up to the current row r */
        cum += r + 1;
        if (cum > n)
            break;
        ++r;
    }
    idx_t prev = cum - (r + 1);   /* entries before this row     */
    idx_t k = n - prev;           /* column within the row       */

    /* Edge cases (the borders of the triangle) */
    if (k == 0 || k == r)
        return (seq_t)1;

    /* Compute the Fibonomial coefficient multiplicatively, canceling
       common factors at each step to keep intermediate values small. */
    seq_t res = (seq_t)1;
    for (idx_t j = 1; j <= k; ++j) {
        seq_t num = fib(2 * (r - j + 1));  /* Fib(2*(n-j))   */
        seq_t den = fib(2 * j);            /* Fib(2*j)       */

        /* Cancel gcd between current result and denominator */
        seq_t g = gcd(res, den);
        res /= g;
        den /= g;

        /* Cancel remaining gcd between numerator and denominator */
        g = gcd(num, den);
        num /= g;
        den /= g;   /* den should now be 1 */

        /* Multiply the reduced numerator */
        res *= num;
    }

    return res;
}
