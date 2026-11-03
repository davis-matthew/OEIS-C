/*
 * A053788  Next prime after n^5.
 * https://oeis.org/A053788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = next prime greater than n^5 (OEIS A053788)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-01)
 * Helpers:   is_prime_seq, mulmod_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mulmod_seq.h"

/* Compute n^5 as a seq_t.  The expression is exact for all seq_t
   types that support 64‑ or 128‑bit arithmetic or arbitrary precision. */
static seq_t pow5_seq_t(seq_t x)
{
    seq_t r = x;
    r *= x;   /* x^2 */
    r *= x;   /* x^3 */
    r *= x;   /* x^4 */
    r *= x;   /* x^5 */
    return r;
}

/* Exponentiation by squaring:  base^exp mod mod. */
static seq_t pow_mod_seq_t(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    base %= mod;
    while (exp > 0)
    {
        if ((exp & 1) != 0)
            result = mulmod_seq(result, base, mod);
        base = mulmod_seq(base, base, mod);
        exp >>= 1;
    }
    return result;
}

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

    while (!is_prime_seq(cand))
        ++cand;                      /* increment until a prime is found */

    return cand;
}
