/*
 * A028573  Expansion of theta_3(z)*theta_3(4z)+theta_2(z)*theta_2(4z).
 * https://oeis.org/A028573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (m,k) of m^2 + 4k^2 = n with
 *            m ≡ k (mod 2)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count all ordered pairs (m,k) ∈ ℤ^2 satisfying the condition. */

#include <stdint.h>
#include <stddef.h>

/* Integer square root for idx_t (assumed unsigned).  Returns floor(sqrt(x)).  */
static inline idx_t isqrt_idx(idx_t x)
{
    idx_t r = 0;
    for (int i = 62; i >= 0; --i) {
        idx_t t = r + (1ull << i);
        /* Use 128‑bit multiplication to avoid overflow. */
        if ((unsigned __int128)t * t <= x) {
            r = t;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t k = 0; ; ++k) {
        unsigned __int128 kk = (unsigned __int128)k * k;
        unsigned __int128 val = 4ull * kk;          /* 4*k*k */
        if (val > n) break;
        idx_t r = n - (idx_t)val;
        idx_t m = isqrt_idx(r);
        if ((unsigned __int128)m * m != r) continue;
        if ((m & 1) != (k & 1)) continue;          /* parity must match */
        if (m == 0 && k == 0) {
            cnt += 1;                              /* (0,0) only one ordering */
        } else if (m == 0 || k == 0) {
            cnt += 2;                              /* one zero, two signs on the other */
        } else {
            cnt += 4;                              /* both non‑zero, four sign combinations */
        }
    }
    return cnt;
}
