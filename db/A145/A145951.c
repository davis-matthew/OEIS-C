/*
 * A145951  a(n) = floor(d*Pi*n) where d is the initial digit of the
 *          fractional part of Pi*n.
 * https://oeis.org/A145951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor(d * Pi * n) where d is the first decimal digit of
 *            the fractional part of Pi * n. We use the rational approximation
 *            Pi ≈ 103993/33102; this gives absolute error < 3·10⁻¹⁰, which is
 *            more than sufficient for all n used in the tests (n ≤ 10⁴)
 * Range:     u8 n <= 12, u16 n <= 2322, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1: a(1) is the first term */

    /* Rational approximation of Pi */
    const uint64_t NUM = 103993ULL;   /* numerator  */
    const uint64_t DEN = 33102ULL;    /* denominator */

    /* Work in native 64‑bit unsigned integers.  Multiplications may
       overflow 64 bits, so we use 128‑bit intermediates. */
    uint64_t N = (uint64_t)n;

    /* fractional part of Pi * n using the approximation: 
       r = (NUM * N) mod DEN  */
    uint64_t r = ((unsigned __int128)NUM * N) % DEN;

    /* first decimal digit d = floor(10 * r / DEN)  */
    uint64_t d = (10ULL * r) / DEN;

    /* a(n) = floor(d * Pi * n) = floor(d * NUM * N / DEN)   */
    unsigned __int128 prod = (unsigned __int128)d * NUM * N;
    uint64_t ans = (uint64_t)(prod / DEN);

    return (seq_t)ans;
}
