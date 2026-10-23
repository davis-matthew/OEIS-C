/*
 * A166626  Totally multiplicative sequence with a(p) = 5p for prime p.
 * https://oeis.org/A166626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * 5^Ω(n) where Ω(n) is the number of prime factors
 *            counted with multiplicity
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used for the sequence offset 1 */
    seq_t omega = bigomega(n);      /* Ω(n) */
    seq_t pow5 = 1;
    while (omega > 0) {
        pow5 *= 5;
        omega -= 1;
    }
    return (seq_t)n * pow5;         /* a(n) = n * 5^Ω(n) */
}
