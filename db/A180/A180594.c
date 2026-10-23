/*
 * A180594  Digital root of 4n.
 * https://oeis.org/A180594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = digital_root(4*n)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Compute (4*n) mod 9 without overflow */
    uint64_t mod = ((n % 9) * 4) % 9;

    /* Digital root: 9 for a non‑zero multiple of 9, otherwise the residue */
    if (mod == 0)
        return (seq_t)9;
    return (seq_t)mod;
}
