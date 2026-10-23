/*
 * A013612  Triangle of coefficients in expansion of (1+5*x)^n.
 * https://oeis.org/A013612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) = 5^k * binomial(r, k) where r = ⌊(√(8*n+1) - 1)/2⌋ k = n -
 *            r*(r+1)/2 This gives the row‑by‑row flattening of the triangle
 *            T(n,k) = 5^k * binomial(n, k) of (1+5*x)^n
 * Range:     u8 n <= 12, u16 n <= 32, u32 n <= 114, u64 n <= 370,
 *            u128 n <= 1365, big any n
 * Helpers:   binomial, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    /* Find the row r using binary search on the triangular numbers */
    idx_t low  = 0;
    idx_t high = n;                 /* n is a non‑negative idx_t */
    idx_t r    = 0;                 /* will hold the largest r with r*(r+1)/2 <= n */

    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        /* compute mid*(mid+1)/2 using 128‑bit arithmetic */
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    /* Compute the triangular number T_r = r*(r+1)/2 safely */
    idx_t T_r = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);

    /* Column k within the row */
    idx_t k = n - T_r;

    /* Compute the value: binomial(r, k) * 5^k */
    seq_t bin = binomial(r, k);
    seq_t pwr = power5(k);
    return bin * pwr;
}
