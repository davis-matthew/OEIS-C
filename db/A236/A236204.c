/*
 * A236204  Numbers not divisible by 2, 3 or 11.
 * https://oeis.org/A236204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 66*⌊(n‑1)/20⌋ + r[(n‑1)%20] where r =
 *            {1,5,7,13,17,19,23,25,29,31,35,37,41,43,47,49,53,59,61,65}
 * Range:     u8 n <= 77, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* offset 1: n ≥ 1 */
    idx_t idx = n - 1;

    /* residues coprime to 66 */
    static const uint8_t residues[20] = {
        1u, 5u, 7u, 13u, 17u, 19u, 23u, 25u, 29u, 31u,
        35u, 37u, 41u, 43u, 47u, 49u, 53u, 59u, 61u, 65u
    };

    idx_t q = idx / 20U;   /* number of full blocks of 20 */
    idx_t r = idx % 20U;   /* index inside the block */

    seq_t val = (seq_t)q * (seq_t)66u;
    val += (seq_t)residues[r];
    return val;
}
