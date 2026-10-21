/*
 * A213077  a(n) = round(n^2 - sqrt(n)).
 * https://oeis.org/A213077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n^2 - sqrt(n))
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned integer (floor). */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t result = 0;
    idx_t bit = 1ULL << 62;          /* highest power of four ≤ 2^64 */
    while (bit > n)
        bit >>= 2;
    while (bit)
    {
        idx_t temp = result + bit;
        if (n >= temp)
        {
            n -= temp;
            result = (result >> 1) + bit;
        }
        else
        {
            result >>= 1;
        }
        bit >>= 2;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t s   = isqrt_u64(n);            /* floor(sqrt(n)) */
    seq_t nn  = (seq_t)n;
    seq_t res = nn * nn;                 /* n^2 */

    /* If sqrt(n) ≤ s + 0.5  →  round up (subtract s)
       otherwise round down (subtract s + 1) */
    if (n <= (idx_t)(s * s + s))
        res -= (seq_t)s;
    else
        res -= (seq_t)(s + 1);

    return res;
}
