/*
 * A201783  Primes of the form 5n^2 - 1.
 * https://oeis.org/A201783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 29, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth prime of the form 5*k^2-1 (k even, starting at 2) */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined offset handling */
    idx_t found = 0;                 /* count of primes found so far */
    seq_t k = 2;                     /* first even k that can give an odd number */

    while (true) {
        seq_t p = 5 * k * k - 1;
        if (is_prime_seq(p)) {
            ++found;
            if (found == n) return p;
        }
        k += 2;                      /* skip odd k, which give even (composite) numbers */
    }
}
