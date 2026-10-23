/*
 * A106562  Perfect squares which are not the sum of two primes.
 * https://oeis.org/A106562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Approximate formula: a(n) is the nth perfect square that is not
 *  representable as the sum of two primes.
 *  The sequence contains all odd squares m = k^2 with k odd (except a(1)=1)
 *  for which m-2 is composite; even squares are omitted because every
 *  even square (except 4) has a Goldbach representation and is not part
 *  of the sequence.  The algorithm simply tests successive odd k,
 *  counts the qualifying squares, and stops when the nth one is found.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)       /* undefined for offset 1; return 0 */
        return 0;

    idx_t   count = 0;   /* number of found terms */
    idx_t   k     = 1;   /* current square root */

    while (true) {
        seq_t m = (seq_t)k * (seq_t)k;   /* the square k² */

        bool good;
        if (k == 1)                         /* a(1)=1 */
            good = true;
        else if ((k & 1) == 0)              /* even k → even square → sum of two primes */
            good = false;
        else {                              /* odd square: check if m-2 is composite */
            uint64_t val = (uint64_t)(m - 2);  /* m fits in 64 bits for our range */
            good = !is_prime(val);
        }

        if (good) {
            ++count;
            if (count == n)
                return m;
        }
        ++k;
    }
}
