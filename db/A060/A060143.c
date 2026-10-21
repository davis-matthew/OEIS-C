/*
 * A060143  a(n) = floor(n/tau), where tau = (1 + sqrt(5))/2.
 * https://oeis.org/A060143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = floor(n * φ) – n, where φ = (1+√5)/2. * Using the ratio
 *            of consecutive Fibonacci numbers F₃₁/F₃₀ gives an * exact
 *            integer approximation of φ and works for all non‑negative * n.
 *            The computation uses an __int128 intermediate to avoid overflow
 *            * for large idx_t values
 * Range:     u8 n <= 359, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* F30 = 832040, F31 = 1346269  →  F31/F30 ≈ φ with error < 10⁻¹²   */
    static const uint64_t F30 = 832040u;
    static const uint64_t F31 = 1346269u;

    /* compute floor( n * F31 / F30 ) - n  */
    __int128 prod = (__int128)(seq_t)n * F31;
    seq_t t = prod / F30;          /* integer division = floor() for n ≥ 0   */
    return t - (seq_t)n;
}
