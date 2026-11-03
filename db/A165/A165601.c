/*
 * A165601  Midpoint height of Jacobi-bridge, computed for 4n+3. a(n) =
 *          Sum_{i=0..(2n+1)} J(i,4n+3), where J(i,m) is the Jacobi symbol.
 * https://oeis.org/A165601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0}^{2 n + 1} J(i, 4 n + 3) where J(i,m) is the
 *            Jacobi symbol
 * Range:     u8 n <= 5641, u16 n <= 269535, u32 n <= 269535, u64 n <= 269535,
 *            u128 n <= 269535, big any n
 * Validated: standard, pass, 103 terms through n = 9754 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The jacobi_symbol function prototype is provided in the utils library. */
#include <stdint.h>
#include <stdbool.h>

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    /* Compute m = 4 n + 3 (always odd). */
    uint64_t m = 4 * n + 3;

    /* Summation range: i = 0 .. 2 n + 1. */
    idx_t max_i = 2 * n + 1;

    /* Use a signed accumulator; the absolute value will never overflow. */
    int64_t sum = 0;

    for (idx_t i = 0; i <= max_i; ++i)
    {
        int j = jacobi_symbol(i, m);
        sum += (int64_t)j;
    }

    /* Convert the signed result to an unsigned seq_t. */
    return (seq_t)sum;
}
