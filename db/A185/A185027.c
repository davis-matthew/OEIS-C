/*
 * A185027  Sum of the triangular divisors of n.
 * https://oeis.org/A185027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{T triangular, T|n} T (trapezoid divisors of n)
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* seq_t, idx_t, and other utilities are supplied by the build environment */

seq_t a(idx_t n)
{
    if (n < 1) return 0;         /* offset is 1, a(0) undefined */
    seq_t sum = 0;
    /* use 128‑bit arithmetic for intermediate triangular numbers */
    __uint128_t t = 1;           /* T_1 */
    for (uint64_t k = 1; ; ++k) {
        if (t > n) break;         /* stop when the triangular exceeds n */
        if (n % (uint64_t)t == 0) /* check divisibility using 64‑bit remainder */
            sum += (seq_t)(uint64_t)t;
        t += k + 1;               /* next triangular T_{k+1} = T_k + (k+1) */
    }
    return sum;
}
