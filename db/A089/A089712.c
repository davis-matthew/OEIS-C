/*
 * A089712  Smallest prime as a concatenation k and k-n.
 * https://oeis.org/A089712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   digit_len_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A089712: a(n) = smallest prime that can be written as
 *   concatenation of an integer k and k-(n-1),
 * with k ≥ max(1, n-1).  The sequence is indexed from 1 (offset 1).  */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_len_seq.h"

/* Compute 10^e in seq_t. */
static seq_t pow10_seq(seq_t e) {
    seq_t r = 1;
    while (e-- > 0) {
        r *= 10;
    }
    return r;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* Convert the 1‑based index to the underlying n₀ = n‑1 used in the
       definition.  */
    if (n == 0) {
        return 0;          /* undefined for n < 1 */
    }
    idx_t n0 = n - 1;      /* n₀ = n-1 (may be 0 for the first term) */

    /* Start searching for k.  k must be at least 1, and k-n₀ must be
       non‑negative, so k ≥ n₀.  The smallest allowable k is:
          k = 1, when n₀ == 0
          k = n₀, when n₀ > 0
       For n₀ > 0, the value k=n₀ produces a trailing part of 0, which
       is handled specially below.  */
    idx_t k = (n0 == 0) ? (idx_t)1 : n0;

    for (;;) {
        idx_t r = k - n0;              /* r = k - (n-1) */
        seq_t p;

        if (r == 0) {                  /* r == 0 → need one more digit (the digit '0') */
            p = (seq_t)k * (seq_t)10;   /* concatenation of k and 0 */
        } else {
            seq_t mult = pow10_seq((seq_t)digit_len_seq((seq_t)r));
            p = (seq_t)k * mult + (seq_t)r;
        }

        if (is_prime_big(p)) {
            return p;                  /* first (hence smallest) prime found */
        }
        ++k;                           /* try next k */
    }
}
