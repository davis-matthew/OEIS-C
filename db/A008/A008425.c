/*
 * A008425  Theta series of {D_6}* lattice.
 * https://oeis.org/A008425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 94, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   kronecker_minus4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1                            for n = 0
 *             a(n) = Σ_{d|n} d² * 4 *
 *              (Kronecker(-4, n/d) - Kronecker(-4, d))   for n > 0
 * where Kronecker(-4, k) = 0 if k even,
 *                         =  1 if k ≡ 1 (mod 4),
 *                         = -1 if k ≡ 3 (mod 4).
 */

#include <stdint.h>
#include "../utils/kronecker_minus4.h"

/* integer sqrt of n (floor), works for idx_t which is unsigned */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid) low = mid;
        else high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* use signed 128‑bit for intermediate accumulation */
    __int128 sum = 0;
    idx_t sqrt_n = isqrt_u64(n);

    for (idx_t i = 1; i <= sqrt_n; ++i) {
        if (n % i == 0) {
            idx_t d = i;
            idx_t e = n / i;
            __int128 d_sq = (__int128)d * d;
            int sym_nd = kronecker_minus4(e);
            int sym_d  = kronecker_minus4(d);
            sum += d_sq * (__int128)(4 * (sym_nd - sym_d));

            if (d != e) {
                __int128 e_sq = (__int128)e * e;
                int sym_ne = kronecker_minus4(d); /* = sym_d */
                int sym_e  = kronecker_minus4(e);
                sum += e_sq * (__int128)(4 * (sym_ne - sym_e));
            }
        }
    }

    return (seq_t)sum;
}
