/*
 * A053788  Next prime after n^5.
 * https://oeis.org/A053788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = next prime greater than n^5 (OEIS A053788)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   is_prime_seq_t, mul_mod_seq_t, pow5_seq_t, pow_mod_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq_t.h"
#include "../utils/mul_mod_seq_t.h"
#include "../utils/pow5_seq_t.h"
#include "../utils/pow_mod_seq_t.h"

/* Deterministic Miller–Rabin for 0 < n < 2^128.
   Uses the first 12 prime bases which are sufficient for all 128‑bit integers
   (see https:

/* Public interface required by the test harness. */
seq_t a(idx_t n)
{
    /* n is at least 1 in the sequence definition */
    seq_t n_seq = (seq_t)n;
    seq_t n5     = pow5_seq_t(n_seq);
    seq_t cand   = n5 + 1;           /* first integer > n^5 */

    while (!is_prime_seq_t(cand))
        ++cand;                      /* increment until a prime is found */

    return cand;
}
