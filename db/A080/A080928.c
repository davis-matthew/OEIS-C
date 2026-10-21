/*
 * A080928  Triangle T(n,k) read by rows: T(n,k) = Sum_{i=0..n}
 *          C(n,2i)*C(2i,k).
 * https://oeis.org/A080928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the nth entry of triangle T(n,k) read by rows; T(n,k) =
 *            C(n,k) * 2^(n-k-1) for k < n, T(n,n) = (n+1) mod 2
 * Range:     u8 n <= 29, u16 n <= 92, u32 n <= 280, u64 n <= 958,
 *            u128 n <= 3593, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Integer square‑root for unsigned 64‑bit values
   (returns floor(sqrt(x))) using only / and comparisons. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= x / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Find the row r containing the nth entry (0‑based).
       r satisfies r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t disc = 1 + 8 * n;                     /* 1 + 8*n */
    idx_t sqrt_d = isqrt_u64(disc);             /* floor(sqrt(disc)) */
    idx_t r = (sqrt_d - 1) / 2;                 /* floor((sqrt-1)/2) */

    /* Offset of the first element of row r. */
    idx_t offset = r * (r + 1) / 2;

    /* Column index within the row. */
    idx_t c = n - offset;                       /* 0 <= c <= r */

    if (c == r) {
        /* Diagonal entry. */
        return (seq_t)((r + 1) & 1U);
    } else {
        /* Off‑diagonal entry. */
        seq_t bin = binomial((idx_t)r, (idx_t)c);
        seq_t pow = pow2((idx_t)(r - c - 1));
        return bin * pow;
    }
}
