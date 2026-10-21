/*
 * A054027  Numbers that do not divide their sum of divisors.
 * https://oeis.org/A054027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   divides_sigma
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A054027: Numbers that do not divide their sum of divisors. */
/* The term a(n) is the n-th integer m ≥ 2 for which m does NOT divide σ(m),
   where σ(m) is the sum of all positive divisors of m. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/divides_sigma.h"

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined for n = 0; guard clause */
        return (seq_t)0;

    idx_t   count = 0;           /* how many valid terms found so far */
    for (uint64_t m = 2; ; ++m) {
        if (!divides_sigma(m)) {  /* m ∤ σ(m) → include in sequence */
            ++count;
            if (count == n)
                return (seq_t)m;   /* cast to seq_t for the build's type */
        }
    }
}
