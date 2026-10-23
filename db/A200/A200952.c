/*
 * A200952  Primes of the form 7n^3-6.
 * https://oeis.org/A200952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   nth prime of the form 7*n^3-6
 * Range:     u8 none, u16 n <= 2, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */
    idx_t found = 0;

    for (idx_t k = 1; ; ++k) {
        seq_t kval = (seq_t)k;
        seq_t k3   = kval * kval;   /* k^2 */
        k3 *= kval;                 /* k^3 */

        seq_t p = (seq_t)7 * k3 - 6;    /* prime candidate */

        if (p < 2) continue;           /* skip non‑positive values */

        if (is_prime_seq(p)) {
            ++found;
            if (found == n) return p;
        }
    }
}
