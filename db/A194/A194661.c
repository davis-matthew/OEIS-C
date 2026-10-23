/*
 * A194661  Triangular array: T(n,k)=[<n*r>+<k*r>], where [ ] = floor, < > =
 *          fractional part, and r = (1+sqrt(5))/2 (the golden ratio).
 * https://oeis.org/A194661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * The sequence is the triangular array T(n,k) = * ⌊{n·φ} +
 *            {k·φ}⌋ (fractional parts of multiples of the golden ratio φ), *
 *            flattened row‑wise (row 1 has one element, row 2 has two, …). *
 *            Since ⌊{x}⌋+⌊{y}⌋ ≤ ⌊{x}+{y}⌋ ≤ ⌊{x}⌋+⌊{y}⌋+1, the value is 0 or
 *            1 and can be * computed as * T(n,k) = ⌊(n+k)·...
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fibonacci convergent of the golden ratio:
 *   φ ≈ F31 / F30 = 1346269 / 832040
 *   (error < 8.4·10⁻⁹, accurate for n ≤ 10 000)
 */
static const uint64_t PHI_NUM = 1346269ULL;
static const uint64_t PHI_DEN = 832040ULL;

/* Integer floor of n · φ   (φ is rational above) */
static inline seq_t floor_phi(idx_t n)
{
    return (seq_t)(((__int128)n * PHI_NUM) / PHI_DEN);
}

/* Integer square root of a 64‑bit unsigned integer */
static inline idx_t floor_isqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (( (__int128)mid * mid ) <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main OEIS routine */
seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≥ n */
    idx_t r = (floor_isqrt(1 + 8 * n) - 1) / 2;
    if ( ( (__int128)r * (r + 1) ) / 2 < n )
        ++r;                               /* safety for rounding */

    /* Start index of this row (1‑based) */
    idx_t start = ( (__int128)(r - 1) * r ) / 2;   /* cumulative count before row r */

    /* Column k (1‑based) */
    idx_t k = n - start;   /* 1 ≤ k ≤ r */

    /* Compute T(r,k) using the floor formula */
    seq_t fr  = floor_phi(r);
    seq_t fk  = floor_phi(k);
    seq_t frk = floor_phi(r + k);

    return frk - fr - fk;   /* 0 or 1 */
}
