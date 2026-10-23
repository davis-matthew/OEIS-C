/*
 * A106476  Sequence array of Euler phi function.
 * https://oeis.org/A106476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = phi(r - (n - r(r+1)/2) + 1) where r = floor((sqrt(1 + 8n)
 *            - 1) / 2) is the row index of n in the triangular array T(n,k) =
 *            phi(n-k+1). This implements the flattening of the triangle
 *            described in A106476
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   phi_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi_simple.h"

/* Compute integer square root of an unsigned __int128 value. */
static unsigned long long isqrt128(unsigned __int128 x)
{
    unsigned long long lo = 0;
    unsigned long long hi = 1ULL << 33;          /* sufficient for all inputs   */
    while (lo < hi) {
        unsigned long long mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Return the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
static idx_t row_of(idx_t n)
{
    unsigned __int128 nn = n;
    unsigned __int128 disc = 1 + 8 * nn;
    unsigned long long sqrt_d = isqrt128(disc);
    idx_t r = (idx_t)((sqrt_d - 1) / 2);

    /* Correct possible off‑by‑one due to integer arithmetic. */
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= nn)
        ++r;
    while ((unsigned __int128)r * (r + 1) / 2 > nn)
        --r;
    return r;
}

/* Main function asked for. */
seq_t a(idx_t n)
{
    /* Find the row r and column k (0‑based) of index n. */
    idx_t r = row_of(n);
    idx_t row_start = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - row_start;                /* column */
    idx_t m = r - k + 1;                    /* argument of phi */

    return phi_simple(m);
}
