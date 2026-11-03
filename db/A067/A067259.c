/*
 * A067259  Cubefree numbers which are not squarefree.
 * https://oeis.org/A067259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th cubefree integer that is not squarefree (search)
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9935 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence contains all integers m such that m is cubefree (no prime factor
   occurs with exponent ≥3) and m is not squarefree (at least one prime
   factor occurs with exponent exactly 2). */

#include <stdint.h>
#include <stdbool.h>

/* Returns true if m is cubefree and not squarefree. */
static bool cubefree_not_squarefree(idx_t m)
{
    idx_t rem = m;
    bool has_square = false;

    /* Check prime factors up to sqrt(rem). */
    for (idx_t p = 2; p * p <= rem; ++p) {
        if (rem % p == 0) {
            int exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
                if (exp >= 3)       /* not cubefree */
                    return false;
            }
            if (exp == 2)
                has_square = true;
        }
    }

    /* rem > 1 means a remaining prime factor with exponent 1. */
    return has_square;
}

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
