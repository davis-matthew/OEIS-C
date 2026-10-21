/*
 * A217193  Number of permutations in S_{n+3} containing an increasing
 *          subsequence of length n.
 * https://oeis.org/A217193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 53, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n^6+6*n^5+10*n^4+4*n^3+19*n^2+62*n+66)/6 for n>1,
 *              a(0) = 6, a(1) = 24.
 */
#include <stdint.h>

/* External linkage function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)6;
    if (n == 1) return (seq_t)24;

    /* use a 128‑bit temporary to avoid overflow on 64‑bit builds */
    __uint128_t m = (__uint128_t)n;
    __uint128_t n2 = m * m;          /* n^2 */
    __uint128_t n3 = n2 * m;         /* n^3 */
    __uint128_t n4 = n3 * m;         /* n^4 */
    __uint128_t n5 = n4 * m;         /* n^5 */
    __uint128_t n6 = n5 * m;         /* n^6 */

    __uint128_t num = n6
                     + 6 * n5
                     + 10 * n4
                     + 4 * n3
                     + 19 * n2
                     + 62 * m
                     + 66;

    num /= 6;                        /* numerator is divisible by 6 for n>1 */

    return (seq_t)num;
}
