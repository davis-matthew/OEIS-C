/*
 * A059536  Beatty sequence for zeta(2)/(zeta(2)-1).
 * https://oeis.org/A059536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * α) with α = ζ(2)/(ζ(2)-1) ≈ 2.5505475
 * Range:     u8 n <= 100, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* α ≈ 2.5505475  ⇒  α ≈ 2550547500000000000 / 1000000000000000000 */
    static const unsigned long long NUM = 2550547500000000000ULL; /* numerator of α */
    static const unsigned long long DEN = 1000000000000000000ULL; /* denominator of α */

    if (n == 0)   /* behaviour below offset is irrelevant */
        return 0;

    /* use 128‑bit arithmetic to avoid overflow: floor(n*NUM/DEN) */
    unsigned __int128 prod = (unsigned __int128)n * NUM;
    unsigned long long result = (unsigned long long)(prod / DEN);

    return (seq_t)result;
}
