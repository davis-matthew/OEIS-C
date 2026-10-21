/*
 * A213963  Number of 3X3 0..n matrices with row and column 1..3 having sum <=
 *          i*n.
 * https://oeis.org/A213963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            (n+3)*(n+2)*(449*n^5+3038*n^4+7759*n^3+9790*n^2+6264*n+1680)*(1+n)^2/10080
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 193, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* cast the index to an unsigned 64‑bit integer */
    uint64_t nn = (uint64_t)n;
    __uint128_t N = nn;            /* use 128‑bit intermediate */

    /* compute polynomial part */
    __uint128_t n2 = N * N;        /* n^2 */
    __uint128_t n3 = n2 * N;       /* n^3 */
    __uint128_t n4 = n3 * N;       /* n^4 */
    __uint128_t n5 = n4 * N;       /* n^5 */

    __uint128_t poly = (__uint128_t)449  * n5
                     + (__uint128_t)3038 * n4
                     + (__uint128_t)7759 * n3
                     + (__uint128_t)9790 * n2
                     + (__uint128_t)6264 * N
                     + (__uint128_t)1680;

    /* compute the prefactor (n+3)(n+2)(n+1)^2 */
    __uint128_t pref = (N + 3) * (N + 2);
    pref *= (N + 1);
    pref *= (N + 1);

    /* full numerator */
    __uint128_t prod = pref * poly;

    /* divide by 10080 (exact division) */
    prod /= (__uint128_t)10080;

    /* cast the 128‑bit intermediate back to seq_t */
    return (seq_t)prod;
}
