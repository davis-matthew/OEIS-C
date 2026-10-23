/*
 * A176622  Primes of the form x^2 + 17*y^2, where x and y=x+1 are consecutive
 *          natural numbers.
 * https://oeis.org/A176622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form x^2 + 17*(x+1)^2, where x runs
 *            over non‑negative integers (x=0 gives a(1)=17)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 10, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation performs a simple search: it tests successive
   values x = 0,1,2,… for primality of  x^2 + 17*(x+1)^2.  The
   function returns the n‑th prime found.  It requires the helper
   primality test is_prime_big defined in ../utils/is_prime_big.h. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset does not matter */

    idx_t found = 0;               /* how many primes have been found */
    idx_t x = 0;                   /* current candidate x value */
    seq_t ans = 0;                 /* last found prime */

    while (found < n) {
        /* compute val = x^2 + 17*(x+1)^2 as seq_t */
        seq_t xx = (seq_t)x * (seq_t)x;
        seq_t y = (seq_t)(x + 1);
        seq_t yy2 = y * y;
        seq_t val = xx + (seq_t)17 * yy2;

        if (is_prime_big(val)) {
            ++found;
            ans = val;
        }
        ++x;
    }
    return ans;
}
