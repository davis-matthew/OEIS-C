/*
 * A038700  Smallest prime == -1 (mod n).
 * https://oeis.org/A038700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime p such that p ≡ -1 (mod n)
 * Range:     u8 n <= 42, u16 n <= 1666, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 79 terms through n = 8366 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. p % n == n-1 (or 0 when n==1). */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* offset 1: a(1) = 2 */
    if (n == 1) {
        return (seq_t)2;
    }

    /* For n >= 2, search for the smallest prime congruent to n-1 modulo n */
    idx_t cand = n - 1;           /* first candidate */
    if (cand < 2) {               /* handle n=2 where cand=1 < 2 */
        cand += n;
    }

    while (!is_prime(cand)) {
        cand += n;                /* next integer ≡ -1 (mod n) */
    }

    return (seq_t)cand;
}
