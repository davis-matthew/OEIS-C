/*
 * A144612  Sturmian word of slope (3-sqrt(3))/2.
 * https://oeis.org/A144612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)r) - floor(n r), where r = (3-sqrt(3))/2. The
 *            values are 0 or 1
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 64‑bit values.
   Returns floor(sqrt(x)). */
static inline unsigned long long isqrt64(unsigned long long x)
{
    unsigned long long res = 0;
    unsigned long long bit = 1ULL << 62;              /* Highest power of four ≤ x. */
    while (bit > x)
        bit >>= 2;
    while (bit)
    {
        if (x >= res + bit)           /* Try to set this bit. */
        {
            x -= res + bit;
            res = (res >> 1) + bit;
        }
        else
            res >>= 1;
        bit >>= 2;
    }
    return res;
}

/* Floor of n * r, where r = (3 - sqrt(3))/2.  Returns a
   64‑bit unsigned integer (the exact value fits in 64 bits for
   all n used in the tests). */
static inline unsigned long long floor_n_r_unsigned(unsigned long long n)
{
    /* n * sqrt(3) = floor_sqrt(3*n^2) + fractional part. */
    unsigned long long sq = n * n;                       /* n^2 fits in 64 bits for tested n. */
    unsigned long long s  = isqrt64(3 * sq);             /* floor(n*sqrt(3)). */
    unsigned long long q  = 3 * n - s;                   /* 3n - floor(n*sqrt(3)). */

    /* The floor of n*r depends on the parity of q: */
    if ((q & 1ULL) == 0)                                 /* q even: floor = q/2 - 1 */
        return q / 2 - 1;
    else                                                 /* q odd: floor = (q-1)/2 */
        return (q - 1) / 2;
}

/* Main sequence function.  idx_t is an unsigned 64‑bit index,
   seq_t is the integer type used for sequence values. */
seq_t a(idx_t n)
{
    unsigned long long f_next = floor_n_r_unsigned((unsigned long long)(n + 1));
    unsigned long long f_cur  = floor_n_r_unsigned((unsigned long long)n);
    unsigned long long diff   = f_next - f_cur;          /* 0 or 1 */
    return (seq_t)diff;
}
