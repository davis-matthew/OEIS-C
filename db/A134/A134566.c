/*
 * A134566  a(n) = least m such that {-m*tau} > {n*tau}, where { } denotes
 *          fractional part and tau = (1 + sqrt(5))/2.
 * https://oeis.org/A134566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m such that {-m*τ} > {n*τ} (τ = (1+√5)/2 ≈
 *            1.6180339887, fractional part denoted { }). This is equivalent
 *            to floor((m+n)*τ) = floor(m*τ) + floor(n*τ). The value a(n) is
 *            always an odd‑indexed Fibonacci number
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned value */
static idx_t isqrt_u128(__int128 x)
{
    idx_t low = 0, high = ((idx_t)1ULL << 32);        /* > sqrt(max) */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __int128 mid2 = (__int128)mid * (__int128)mid;
        if (mid2 <= x) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* floor(k * τ) for k >= 0
   τ = (1+√5)/2
   floor(k*τ) = floor((k + floor(k*√5)) / 2)
   floor(k*√5) = floor( sqrt(5 * k^2) )
*/
static seq_t floor_phi(idx_t k)
{
    __int128 prod = (__int128)5 * (__int128)k * (__int128)k;
    idx_t s = isqrt_u128(prod);              /* floor(k*√5) */
    return (seq_t)((k + s) / 2);             /* floor((k+s)/2) */
}

/* a(n) – minimal m such that { -m*τ } > { n*τ } */
seq_t a(idx_t n)
{
    seq_t fn = floor_phi(n);                 /* floor(n*τ) */

    for (idx_t m = 1; ; ++m) {
        seq_t fm = floor_phi(m);             /* floor(m*τ) */
        seq_t fm_n = floor_phi(n + m);       /* floor((m+n)*τ) */

        if (fm_n == fm + fn) {
            return (seq_t)m;                 /* a(n) = m  */
        }
    }
}
