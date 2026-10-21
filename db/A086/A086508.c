/*
 * A086508  If n is even, a(n) = smallest prime == -1 (mod n), If n is odd,
 *          a(n) = smallest prime == 1 (mod n).
 * https://oeis.org/A086508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the smallest prime p such that
   p ≡ -1 (mod n)  if n is even,
   p ≡  1 (mod n)  if n is odd. */

#include "../utils/is_prime_big.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* offset starts at 1 – behaviour for n<1 is irrelevant */

    if ((n & 1) == 0) {          /* n even : look for p ≡ -1 (mod n) */
        for (idx_t k = 1;; ++k) {
            seq_t cand = (seq_t)k * (seq_t)n - 1;
            if (cand < (seq_t)2) continue;
            if (is_prime_big(cand)) return cand;
        }
    } else {                       /* n odd : look for p ≡ 1 (mod n) */
        for (idx_t k = 1;; ++k) {
            seq_t cand = (seq_t)k * (seq_t)n + 1;
            if (cand < (seq_t)2) continue;
            if (is_prime_big(cand)) return cand;
        }
    }
}
