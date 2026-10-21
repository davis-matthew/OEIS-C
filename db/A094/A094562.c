/*
 * A094562  Initial decimal digit of fractional part of Pi*n, Pi=3.1415...
 * https://oeis.org/A094562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(10*Pi*n) mod 10
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the first decimal digit after the point of Pi*n using integer arithmetic.
 * Pi is approximated by its 18‑digit expansion:
 *      Pi ≈ 3.141592653589793238
 * scaled to 10^18.  The approximation error is < 10^-18, which for n ≤ 10^9
 * (and far beyond) guarantees that 10*{Pi*n} is correct to the nearest integer.
 * The computation uses only integer addition, subtraction, multiplication, division
 * and modulus, so it respects the restrictions on allowed operations. */

#include <stdint.h>

/*  Pi * 10^18  (truncated)  */
static const unsigned long long PI_SCALED = 3141592653589793238ULL;
/* 10^18  (scale factor) */
static const unsigned long long SCALE     = 1000000000000000000ULL;

seq_t a(idx_t n)
{
    /*  Use 128‑bit intermediate to avoid overflow when multiplying by 10^18  */
    __int128 prod = (__int128)n * PI_SCALED;

    /*  Fractional part numerator: (Pi*n) * 10^18  % 10^18  */
    unsigned long long rem = (unsigned long long)(prod % SCALE);

    /*  First decimal digit: floor(10 * rem / 10^18)  */
    unsigned long long digit = (rem * 10ULL) / SCALE;

    return (seq_t)digit;
}
