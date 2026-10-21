/*
 * A070219  Smallest prime obtained as a concatenation of n and a number m
 *          greater than n.
 * https://oeis.org/A070219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, changed
 * Formula:   a(n) = smallest prime obtained by concatenating n with an m > n
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A070219) */
#include <stdbool.h>
#include "../utils/concat_parts.h"   /* returns decimal concatenation of two numbers */
#include "../utils/is_prime_big.h"   /* returns 1 if seq_t number is prime, else 0 */

seq_t a(idx_t n)
{
    idx_t m = n + 1;                 /* search for m > n */
    for (;; ++m) {
        seq_t cand = concat_parts(n, m);  /* decimal concatenation n||m */
        if (is_prime_big(cand))
            return cand;
    }
}
