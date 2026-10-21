/*
 * A227868  Composite numbers congruent to 7 or 23 (mod 30).
 * https://oeis.org/A227868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1932, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Composite numbers congruent to 7 or 23 (mod 30)
   a(1) is the first composite of that form, a(n) is the nth such number. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0; return 0 as sentinel */

    idx_t found = 0;
    seq_t cand7 = 7;                /* candidates ≡ 7 (mod 30)   */
    seq_t cand23 = 23;              /* candidates ≡ 23 (mod 30)  */

    while (1) {
        /* test candidate ≡ 7 (mod 30) */
        if (!is_prime_big(cand7)) {
            found++;
            if (found == n) return cand7;
        }

        /* test candidate ≡ 23 (mod 30) */
        if (!is_prime_big(cand23)) {
            found++;
            if (found == n) return cand23;
        }

        /* advance to next pair of candidates */
        cand7 += 30;
        cand23 += 30;
    }
}
