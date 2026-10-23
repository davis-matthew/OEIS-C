/*
 * A201787  Primes of the form 5n^2 - 6.
 * https://oeis.org/A201787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 5*n^2 - 6 (OEIS A201787) */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, undefined for n < 1 */
    idx_t count = 0;
    idx_t m = 1;                   /* candidate n in the form 5*n^2-6 */

    for (;; ++m)
    {
        seq_t mt = (seq_t)m;
        seq_t val = (seq_t)5 * mt * mt - (seq_t)6;  /* 5*m^2 - 6 */
        if (is_prime_seq(val))
        {
            ++count;
            if (count == n)
                return val;
        }
    }
}
