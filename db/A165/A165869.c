/*
 * A165869  Totally multiplicative sequence with a(p) = 48.
 * https://oeis.org/A165869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 48^Ω(n) (Ω = bigomega, the total number of prime factors
 *            counted with multiplicity)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 63, u64 n <= 4095,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* sequence is defined for n ≥ 1; safe fallback for any n < 1 */
    seq_t omega = bigomega(n);      /* compute Ω(n) */
    return big_pow((seq_t)48, omega); /* compute 48^Ω(n) */
}
