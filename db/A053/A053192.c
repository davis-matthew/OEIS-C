/*
 * A053192  a(n) is the cototient of n^3.
 * https://oeis.org/A053192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (n - φ(n)) (n ≥ 1)
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the cototient of n^3 using the identity
   cototient(n^3) = n^2 * cototient(n).  φ(n) is Euler's totient. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/phi_u64.h"

/* External linkage sequence function */
seq_t a(idx_t n)
{
    /* Offset 1 in OEIS: a(1) is the first term.  For n == 0 return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Compute φ(n) */
    seq_t phi = phi_u64(n);

    /* Cototient of n is n - φ(n) */
    seq_t cot   = (seq_t)n - phi;

    /* n^2 */
    seq_t nsq   = (seq_t)n * (seq_t)n;

    /* a(n) = n^2 * cot */
    return nsq * cot;
}
