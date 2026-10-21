/*
 * A066560  Smallest composite number divisible by n.
 * https://oeis.org/A066560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 if n = 1; a(n) = n if n is composite; a(n) = 2*n if n
 *            is prime (n ≥ 2)
 * Range:     u8 n <= 130, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime64.h"

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)0;          /* undefined for offset 1, but safe fallback */
    if (n == 1U) return (seq_t)4;

    if (is_prime64((uint64_t)n))
        return (seq_t)(n << 1);            /* 2*n, using shift to avoid overflow on seq_t */
    else
        return (seq_t)n;                   /* n is composite, already the smallest composite divisor */
}
