/*
 * A203464  Numbers n such that 65 divides 4n^2 + 1; alternately, numbers
 *          which are 4, 9, 56, or 61 mod 65.
 * https://oeis.org/A203464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 4034, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A203464: a(n) = 65*⌊(n-1)/4⌋ + {4,9,56,61}[(n-1) mod 4] */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute k = floor((n-1)/4) and r = (n-1) mod 4 */
    idx_t k = (n - 1) / 4;
    unsigned r = (unsigned)((n - 1) % 4);

    /* offsets for the four residue classes modulo 65 */
    static const seq_t offsets[4] = {(seq_t)4, (seq_t)9, (seq_t)56, (seq_t)61};

    /* base value 65*k and add the appropriate offset */
    seq_t base = (seq_t)65 * (seq_t)k;
    return base + offsets[r];
}
