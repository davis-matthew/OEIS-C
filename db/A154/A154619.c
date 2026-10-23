/*
 * A154619  Primes of the form (4k^2 + 4k - 5)/5.
 * https://oeis.org/A154619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 34, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154619: primes of the form (4k^2 + 4k - 5)/5.
   k must satisfy k ≡ 0 or 4 (mod 5), so k = 5t or 5t+4 for t ≥ 1. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

static inline seq_t compute_value(seq_t k)
{
    /* Compute (4k^2 + 4k - 5) / 5 */
    seq_t val = ((seq_t)4 * k * k + (seq_t)4 * k - (seq_t)5) / (seq_t)5;
    return val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, but return 0 */
    idx_t count = 0;
    idx_t t = 1;

    for (;; ++t)
    {
        /* k = 5t */
        seq_t k = (seq_t)5 * t;
        seq_t val = compute_value(k);
        if (is_prime_big(val))
        {
            ++count;
            if (count == n) return val;
        }

        /* k = 5t + 4 */
        k = (seq_t)5 * t + (seq_t)4;
        val = compute_value(k);
        if (is_prime_big(val))
        {
            ++count;
            if (count == n) return val;
        }
    }
}
