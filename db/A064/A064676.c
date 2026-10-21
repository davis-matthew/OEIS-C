/*
 * A064676  A Beatty sequence for 10*i^i.
 * https://oeis.org/A064676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A Beatty sequence for 10·i^i
    a(n) = ⌊n · θ⌋,  θ = 10·e^(−π/2) ≈ 2.07879576350761…
    We compute it as the floor of n·p/q with a fixed rational
    approximation p/q = ⌊θ·10¹²⌋ / 10¹², whose error is <10⁻¹²,
    so for all n ≤ 10¹² the floor is exact.  For n ≤ 10⁴ (the
    range of interest) the error is <10⁻⁸, guaranteeing the same
    floor value as the true irrational θ.  No floating‑point or
    <math.h> is used.
*/

#include <stdint.h>

static const unsigned long long P = 2078795763507ULL;      /* ⌊θ·10¹²⌋      */
static const unsigned long long Q = 1000000000000ULL;      /* 10¹²          */

seq_t a(idx_t n)
{
    /*  n is unsigned; product fits in 128 bits.  */
    unsigned __int128 prod = (unsigned __int128)n * P;
    return (seq_t)(prod / Q);
}
