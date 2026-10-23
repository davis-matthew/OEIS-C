/*
 * A134569  a(n) = least m such that {-m*r} < {n*r}, where { } denotes
 *          fractional part and r = sqrt(2).
 * https://oeis.org/A134569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A134569: a(n) = least m such that { -m*sqrt(2) } < { n*sqrt(2) }
   The inequality is equivalent to floor((m+n)*sqrt(2)) = 
   floor(m*sqrt(2)) + floor(n*sqrt(2)) + 1.
   For each n the minimal m satisfies this.  We compute the floor
   values exactly using integer arithmetic: floor(k*sqrt(2)) = floor(sqrt(2*k^2))
   and determine the first m that fulfills the equation.  */
   
#include <stdint.h>

typedef unsigned __int128 u128;

/* Compute floor(sqrt(2)*k) exactly using 128‑bit arithmetic.
   We evaluate floor(sqrt(2*k^2)).  */
static inline seq_t sqrt2_floor_mul_idx(idx_t k)
{
    u128 v = (u128)k * (u128)k * (u128)2;          /* v = 2*k*k  */
    u128 low = 0;
    u128 high = (u128)1 << 64;                     /* high > sqrt(v) for all k <= 2^64-1 */

    while (low < high) {
        u128 mid = (low + high + 1) >> 1;
        if (mid * mid <= v)
            low = mid;
        else
            high = mid - 1;
    }
    return (seq_t)low;                             /* floor(sqrt(2)*k) */
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, but return 0 to avoid UB */
        return (seq_t)0;

    seq_t fn = sqrt2_floor_mul_idx(n);              /* floor(n√2) */

    for (idx_t m = 1; ; ++m) {
        seq_t fm   = sqrt2_floor_mul_idx(m);        /* floor(m√2)   */
        seq_t fmnp = sqrt2_floor_mul_idx(m + n);    /* floor((m+n)√2) */

        if (fmnp == fm + fn + 1)                    /* condition satisfied */
            return (seq_t)m;
    }
}
