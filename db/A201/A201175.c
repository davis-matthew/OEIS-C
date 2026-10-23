/*
 * A201175  Primes of the form 5n^3+6.
 * https://oeis.org/A201175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   generate primes of the form 5*n^3+6 by brute‑force search
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* undefined for offset 0 */
    }

    idx_t count = 0;  /* how many primes found */
    idx_t m     = 1;  /* current n for expression 5*n^3+6 */

    while (true) {
        seq_t nn  = (seq_t)m;           /* cast to seq_t for arithmetic */
        seq_t val = ((seq_t)5) * nn * nn * nn + (seq_t)6;  /* 5*n^3+6 */

        if (is_prime_seq(val)) {          /* primality test */
            ++count;
            if (count == n) {
                return val;              /* desired element */
            }
        }
        ++m;                              /* next n to test */
    }
}
