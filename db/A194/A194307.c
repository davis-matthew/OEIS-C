/*
 * A194307  Triangular array: g(n,k) = number of fractional parts (i*Pi) in
 *          interval [(k-1)/n, k/n], for 1 <= i <= n^2, 1 <= k <= n.
 * https://oeis.org/A194307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of i (1 ≤ i ≤ r²) such that fractional part of i·π
 *            lies in [(k-1)/r, k/r), where r and k are obtained from n by
 *            unfolding the triangular array
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* high–precision rational approximation of π:
 * π ≈ 104348/33215  (error < 1e‑9, enough for the required range). */
static const uint64_t PI_NUM = 104348ULL;
static const uint64_t PI_DEN = 33215ULL;

seq_t a(idx_t n)
{
    /* Convert the linear index n into row r (=i) and column k (=j). */
    idx_t r = 1;
    idx_t cum = 1;                 /* cumulative count up to current row */
    while (cum < n) {
        ++r;
        cum += r;
    }
    idx_t k = n - (cum - r);       /* 1‑based column within row r */

    /* Count the desired fractional parts.  For each i (1 ≤ i ≤ r²),
       compute floor(i·r·π) and look at its residue modulo r.  The
       residue equals k‑1 exactly when the fractional part lies in
       the required interval. */
    seq_t count = 0;
    const uint64_t k_minus_one = k - 1;

    for (idx_t i = 1; i <= r * r; ++i) {
        /* Use 128‑bit arithmetic to avoid overflow.  The intermediate
           product i·r·PI_NUM fits comfortably in 128 bits. */
        __int128 prod = (__int128)i * r * PI_NUM;
        uint64_t val   = (uint64_t)(prod / PI_DEN);   /* floor(i·r·π) */
        if ((val % r) == k_minus_one) {
            ++count;
        }
    }
    return count;
}
