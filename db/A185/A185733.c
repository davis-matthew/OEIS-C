/*
 * A185733  Second accumulation array of the polygonal number array (A086270),
 *          by antidiagonals.
 * https://oeis.org/A185733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by 1) mapping the index n to coordinates
 *            (n',k') on the 2‑D array of the polygonal numbers (A086270) by
 *            antidiagonals (first term is (1,1)); 2) evaluating the closed
 *            form T(n,k) = k*(k+1)*(k+2)*n*(n+1)*(k*n - n + 2*k + 10) / 144
 *            for those coordinates. This implements the OEIS des...
 * Range:     u8 n <= 17, u16 n <= 142, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned long long isqrt_u128(unsigned __int128 x)
{
    unsigned long long low = 0;
    unsigned long long high = (unsigned long long)1 << 63;
    while (low <= high) {
        unsigned long long mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* sequence is defined for n >= 1 (offset 1) */
    if (n == 0)
        return (seq_t)0;

    /* Find diagonal s such that C(s-1) < n <= C(s),  C(s) = (s-1)*s/2 */
    unsigned __int128 t = (unsigned __int128)8 * n + 1;
    unsigned long long root = isqrt_u128(t);
    unsigned long long s = (root + 1) / 2;   /* floor((1+root)/2) */
    if ((unsigned __int128)s * (s - 1) / 2 < n)
        ++s;

    /* index within the diagonal (1-indexed) */
    unsigned __int128 c_prev = (unsigned __int128)(s - 1) * (s - 2) / 2;
    idx_t p = n - (idx_t)c_prev;               /* 1 <= p <= s-1 */

    unsigned long long k_val = s - p;          /* k coordinate */
    unsigned long long n_val = p;              /* n coordinate */

    /* Compute T(n,k) using 128‑bit arithmetic to avoid overflow */
    unsigned __int128 prod = (unsigned __int128)k_val * (k_val + 1) * (k_val + 2);
    prod *= (unsigned __int128)n_val * (n_val + 1);

    unsigned __int128 kn = (unsigned __int128)k_val * n_val;
    unsigned __int128 t3 = kn - n_val + (unsigned __int128)(2 * k_val) + 10;
    prod *= t3;

    prod /= 144;

    return (seq_t)prod;
}
