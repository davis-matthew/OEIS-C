/*
 * A190248  a(n) = [nu+nv+nw]-[nu]-[nv]-[nw], where u=(1+sqrt(5))/2, v=u^2,
 *          w=u^3, []=floor.
 * https://oeis.org/A190248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(4nφ) - 2*floor(nφ) - floor(2nφ), where φ =
 *            (1+sqrt(5))/2. The function computes the floor of k·φ via
 *            integer arithmetic without floating‑point
 * Range:     u8 n <= 28, u16 n <= 8035, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128.
   Returns floor(sqrt(x)).  Uses binary search on uint64_t range. */
static uint64_t isqrt_uint128(unsigned __int128 x)
{
    uint64_t low = 0;
    uint64_t high = (uint64_t)1ULL << 63;  /* sqrt(max 128‑bit) < 2^63 */
    while (low <= high)
    {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* floor(k·φ) where φ = (1 + sqrt(5)) / 2.
   Uses the identity: floor(kφ) = (k + floor(sqrt(5k²))) / 2. */
static seq_t floor_phi(idx_t k)
{
    if (k == 0) return 0;
    unsigned __int128 val = (unsigned __int128)k * k * 5;
    uint64_t sq = isqrt_uint128(val);                 /* floor(sqrt(5k²)) */
    return (seq_t)(( (seq_t)k + (seq_t)sq ) / 2);    /* (k + sq)/2  */
}

/* Sequence function a(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* behaviour for n < 1 is irrelevant */

    seq_t f_n   = floor_phi(n);
    seq_t f_2n  = floor_phi(n * 2);
    seq_t f_4n  = floor_phi(n * 4);

    return f_4n - 2 * f_n - f_2n;
}
