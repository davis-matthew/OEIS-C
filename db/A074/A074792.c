/*
 * A074792  Least k > 1 such that k^n == 1 (mod n).
 * https://oeis.org/A074792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>1 such that k^n == 1 (mod n)
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* n == 1 is a special case */
    if (n == 1)
        return (seq_t)2;

    seq_t mod = (seq_t)n;   /* modulus for exponentiation */
    /* Search k from 2 up to n+1. By Euler's theorem k = n+1 always works. */
    for (idx_t k = 2; k <= n + 1; ++k) {
        seq_t r = mod_pow((seq_t)k, n, mod);
        if (r == 1)
            return (seq_t)k;
    }

    /* The loop should always terminate but return 0 as a defensive fallback */
    return (seq_t)0;
}
