/*
 * A235363  (1 + Sum_{k=1..m-1} k^(m-1)) (mod m), for m = 1, 3, 5, 7, 9, ...
 * https://oeis.org/A235363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1 + ∑_{k=1}^{2n} k^{2n}) mod (2n+1)
 * Range:     u8 n <= 71, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS sequence A235363 using modular
   exponentiation. It works for any seq_t and idx_t sizes
   supported by the build environment. */
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* m = 2n + 1 (the odd modulus) */
    seq_t m = (seq_t)n * 2 + 1;

    /* sum starts with 1, calcuated modulo m so that
       the special case m == 1 yields 0 as required. */
    seq_t sum = 1 % m;

    /* exponent is m - 1 = 2n */
    uint64_t exp = (uint64_t)(m - 1);

    /* limit = m - 1, but cast to idx_t to obey loop bounds */
    idx_t limit = (idx_t)m;

    for (idx_t k = 1; k < limit; ++k) {
        /* Compute k^exp (mod m) efficiently */
        seq_t term = mod_pow((seq_t)k, exp, m);

        /* Add term modulo m */
        sum += term;
        if (sum >= m)
            sum -= m;
    }

    /* Ensure a(n) is in [0, m-1] */
    return sum % m;
}
