/*
 * A109944  a(1) = 1, a(n) = smallest prime number beginning with n if n is
 *          composite or vice versa. If n is prime a(n) = 10n.
 * https://oeis.org/A109944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1)=1; if n is prime then a(n)=10*n; otherwise a(n) is the *
 *            smallest prime whose decimal representation starts with the
 *            digits of n
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"
#include "../utils/is_prime_u64.h"

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    if (is_prime_u64((uint64_t)n))
        return ((seq_t)10 * (seq_t)n);

    /* n is composite: find the smallest prime starting with n. */
    for (idx_t t = 1;; ++t)
    {
        seq_t pow10t = pow10((idx_t)t);
        seq_t lower  = (seq_t)n * pow10t;
        seq_t upper  = (((seq_t)n + (seq_t)1) * pow10t) - (seq_t)1;

        for (seq_t val = lower; val <= upper; val += (seq_t)1)
        {
            if (is_prime_seq(val))
                return val;
        }
    }
}
