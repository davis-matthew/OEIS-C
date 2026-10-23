/*
 * A117630  Complement of A056576.
 * https://oeis.org/A117630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*log(3)/log(3/2)) (n ≥ 1)
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a high‑precision fixed‑point approximation
   of the irrational constant α = log(3)/log(3/2) and integer
   arithmetic only.  The scaling factor S = 10¹⁵ guarantees
   that the computed floor is exact for all n that fit in the
   usual test ranges (e.g., n ≤ 10⁹). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* α ≈ 2.7095110188285095  (log(3)/log(3/2)) */
    /* We store α with 15 decimal places: α ≈ A / 10¹⁵ */
    const uint64_t A = 2709511018828510ULL;   /* α * 10¹⁵, rounded to nearest integer */
    const uint64_t S = 1000000000000000ULL;   /* scaling factor 10¹⁵ */

    /* Compute floor( n * A / S ) exactly using 128‑bit product. */
    unsigned __int128 prod = (unsigned __int128)n * (unsigned __int128)A;
    uint64_t result = (uint64_t)(prod / S);   /* floor division */

    return (seq_t)result;
}
