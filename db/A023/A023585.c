/*
 * A023585  Least prime divisor of 2*prime(n)-1.
 * https://oeis.org/A023585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime divisor of (2*prime(n)-1)
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n >= 1 */
    uint32_t p = get_prime(n);                 /* nth prime as uint32_t */
    seq_t M = (seq_t)2 * (seq_t)p - (seq_t)1;   /* 2*prime(n)-1, always odd */

    seq_t limit = floor_sqrt(M);                /* floor(sqrt(M)) */
    for (seq_t d = (seq_t)3; d <= limit; d += (seq_t)2)
    {
        if (M % d == (seq_t)0)                 /* d divides M */
            return d;
    }
    /* No divisor found: M itself is prime (>=3) */
    return M;
}
