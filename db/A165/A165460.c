/*
 * A165460  The height at the 1/3 point of Jacobi-bridge, computed for 12n+7.
 *          a(n) = Sum_{i=0..(4n+2)} J(i,12n+7), where J(i,m) is the Jacobi
 *          symbol.
 * https://oeis.org/A165460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..(4n+2)} J(i,12n+7)
 * Range:     u8 n <= 1641, u16 n <= 21845, u32 n <= 21845, u64 n <= 21845,
 *            u128 n <= 21845, big any n
 * Validated: standard, pass, 97 terms through n = 9818 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    // m = 12n + 7 (always odd)
    idx_t m   = 12 * n + 7;
    // last term index
    idx_t end = 4 * n + 2;

    int sum = 0;
    for (idx_t i = 0; i <= end; ++i) {
        sum += jacobi_symbol(i, m);
    }
    return (seq_t)sum;
}
