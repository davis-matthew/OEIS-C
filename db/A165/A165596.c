/*
 * A165596  Jacobi symbol (n,59881).
 * https://oeis.org/A165596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Jacobi symbol (n,59881)
 * Range:     i8 n <= 59881, i16 n <= 59881, i32 n <= 59881, i64 n <= 59881,
 *            i128 n <= 59881, big any n
 * Validated: standard, pass, 102 terms through n = 9737 (2026-11-01)
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
    return (seq_t)jacobi_symbol((uint64_t)n, 59881ULL);
}
