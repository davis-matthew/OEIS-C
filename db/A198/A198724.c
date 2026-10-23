/*
 * A198724  Let P(n) be the maximal prime divisor of 3*n+1. Then a(n) is the
 *          smallest number of iterations of P(n) such that the a(n)-th
 *          iteration < n, and a(n) = 0, if such number does not exist.
 * https://oeis.org/A198724
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest k such that iterating m -> largest prime
 *            divisor of (3*m+1) k times falls below n. If no such k exists,
 *            a(n)=0. (Offset 3: a(3)=2, a(4)=3, …)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/largest_prime_divisor.h"   /* uint64_t largest_prime_divisor(uint64_t) */

seq_t a(idx_t n)
{
    /* Defined only for n ≥ 3; below offset return 0 */
    if (n < 3ULL)
        return (seq_t)0;

    uint64_t m = (uint64_t)n;      /* current value of the iteration */
    /* Safely compute 3*m+1 using 128‑bit arithmetic */
    for (unsigned int step = 1; step < 1000; ++step) {
        __uint128_t tmp = (__uint128_t)3 * m + 1;
        if (tmp > UINT64_MAX)   /* cannot process numbers larger than 64 bits */
            break;

        uint64_t val = (uint64_t)tmp;
        uint64_t p   = largest_prime_divisor(val);   /* prime divisor of 3*m+1 */

        if (p < n)                /* first iteration to fall below n */
            return (seq_t)step;
        if (p == m)               /* fixed point – will never drop below n */
            break;
        m = p;                    /* continue with next iteration */
    }

    return (seq_t)0;   /* no such iteration found within the limit */
}
