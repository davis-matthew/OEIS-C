/*
 * A067259  Cubefree numbers which are not squarefree.
 * https://oeis.org/A067259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th cubefree integer that is not squarefree (search)
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cubefree_not_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence contains all integers m such that m is cubefree (no prime factor
   occurs with exponent ≥3) and m is not squarefree (at least one prime
   factor occurs with exponent exactly 2). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/cubefree_not_squarefree.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for index 0 */

    idx_t count    = 0;  /* number of terms found so far */
    idx_t candidate = 4; /* smallest integer that can satisfy the conditions */

    while (true) {
        if (cubefree_not_squarefree(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
