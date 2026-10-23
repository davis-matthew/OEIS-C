/*
 * A100698  Primes of the form k^3 - k + 1.
 * https://oeis.org/A100698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime of the form k^3 - k + 1 (OEIS A100698)
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"


seq_t a(idx_t n)
{
    if (n < 1)      /* undefined for n = 0, but return 0 safely */
        return 0;

    idx_t count = 0;

    for (idx_t k = 1; ; ++k) {
        seq_t kk = (seq_t)k;
        seq_t val = kk * kk * kk;  /* k^3 */
        val -= kk;                 /* k^3 - k */
        ++val;                     /* k^3 - k + 1 */

        if (is_prime_seq(val)) {
            ++count;
            if (count == n)
                return val;
        }
    }
}
