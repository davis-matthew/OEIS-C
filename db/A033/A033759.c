/*
 * A033759  Expansion of Product_{d | 47} theta_3(q^d).
 * https://oeis.org/A033759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥0, k²≤n} ( (k==0?1:2) * ( n‑k²==0?1 : ( (n‑k²)%47==0
 *            && ((n‑k²)/47) is non‑zero square ? 2 : 0) ) )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root (floor) for 64‑bit unsigned integers. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, mid = 0, ans = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        __int128 prod = (__int128)mid * (__int128)mid;
        if (prod <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Test whether a 64‑bit unsigned integer is a perfect square. */
static inline int is_square_u64(idx_t x)
{
    if (x == 0) return 1;                       /* 0 is a square but handled separately */
    idx_t r = isqrt_u64(x);
    return (__int128)r * (__int128)r == x;
}

/* OEIS sequence A033759: expansion of product_{d|47} theta_3(q^d). */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_k = isqrt_u64(n);
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t i = (idx_t)((__int128)k * (__int128)k);
        int coeff1 = (i == 0) ? 1 : 2;          /* coefficient from theta_3(q)   */
        idx_t j = n - i;
        int coeff2 = 0;                         /* coefficient from theta_3(q^47) */
        if (j == 0) {
            coeff2 = 1;
        } else if (j % 47 == 0) {
            idx_t t = j / 47;
            if (t > 0 && is_square_u64(t))
                coeff2 = 2;
        }
        if (coeff2)
            sum += (seq_t)coeff1 * (seq_t)coeff2;
    }
    return sum;
}
