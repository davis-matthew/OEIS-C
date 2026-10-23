/*
 * A134660  Number of odd coefficients in (1 + x + x^2 + x^3)^n.
 * https://oeis.org/A134660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^popcount(3n)
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 3*n using 64‑bit arithmetic. For the expected input range
       (up to about 10 000) this is safe; the multiplication is performed
       in an unsigned 64‑bit type. */
    uint64_t three_n = n * 3ull;

    /* Count the number of 1 bits in 3*n.  __builtin_popcountll is
       available in both C11 and C++17 compilers for 64‑bit values. */
    unsigned int bits = __builtin_popcountll(three_n);

    /* Return 2^bits; pow2() returns the value in the seq_t type. */
    return pow2((idx_t)bits);
}
