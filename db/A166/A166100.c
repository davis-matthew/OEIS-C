/*
 * A166100  Sum of those positive i <= 2n+1, for which J(i,2n+1)=+1. Here
 *          J(i,k) is the Jacobi symbol.
 * https://oeis.org/A166100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 259, u32 n <= 47123, u64 n <= 131071,
 *            u128 n <= 131071, big any n
 * Validated: standard, pass, 67 terms through n = 8749 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = sum of all 1 ≤ i ≤ 2n+1 with Jacobi symbol J(i,2n+1) = +1 */
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
    idx_t odd = (idx_t)2 * n + 1;               /* odd = 2n+1 */
    if (odd == 1) {                            /* special case: modulus 1 */
        /* By convention J(i,1) = 1 for every i, so the sum is just i=1 */
        return (seq_t)1;
    }

    seq_t sum = 0;
    for (idx_t i = 1; i <= odd; ++i) {
        if (jacobi_symbol((uint64_t)i, odd) == 1) {
            sum += (seq_t)i;
        }
    }
    return sum;
}
