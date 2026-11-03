/*
 * A095269  Diving index of 4n+3.
 * https://oeis.org/A095269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the diving index of 4n+3 by brute‑force search:
   a(n) = the smallest u > 1 with Σ_{i=1..u} J(i/(4n+3)) = -1.
   If no such u exists, returns 0. */
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
    uint64_t m = (uint64_t)4 * n + 3;   /* odd number 4n+3 */
    int sum = 0;
    for (uint64_t i = 1; i <= m; ++i) {
        int j = jacobi_symbol(i, m);   /* -1, 0, or 1 */
        sum += j;
        if (sum == -1 && i > 1)
            return (seq_t)i;
    }
    return (seq_t)0;
}
