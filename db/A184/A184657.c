/*
 * A184657  floor(n*s+h-h*s), where s=(3+sqrt(5))/2, h=-1/2; complement of
 *          A184656.
 * https://oeis.org/A184657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3*(2n+1)-2 + floor(sqrt(5*(2n+1)²)))/4)
 * Range:     u8 n <= 97, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t used in the helper */

static unsigned long long int_sqrt_ull(unsigned long long x)
{
    /* Integer square‑root, returns floor(sqrt(x)).  */
    unsigned long long res = 0;
    unsigned long long bit = 1ULL << 62;   /* largest power of two ≤ 2^63 */

    while (bit > x) bit >>= 2;            /* shift until bit <= x */

    while (bit)
    {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 (offset is 1) */
    unsigned long long k = 2ULL * n + 1ULL;                 /* k = 2n+1  */
    unsigned long long val = 5ULL * k * k;                  /* val = 5k²  */
    unsigned long long s5  = int_sqrt_ull(val);             /* floor(k*sqrt5) */
    unsigned long long m   = 3ULL * k - 2ULL;               /* m = 3k-2   */
    return (seq_t)((m + s5) / 4ULL);
}
