/*
 * A225190  a(n) = (n+2)^(n+2) mod n^n.
 * https://oeis.org/A225190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = (n+2)^(n+2) mod n^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   mod_pow, powmod_small_uint64, powmod_uint64_mod2_64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/mod_pow.h"
#include "../utils/powmod_small_uint64.h"
#include "../utils/powmod_uint64_mod2_64.h"

seq_t a(idx_t n)
{
    /* handle the defined offset */
    if (n == 0)
        return 0;

    /* small integer implementation (fits in 64 bits) */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        /* n <= 15  ->  n^n fits in 64 bits */
        if (n <= 15) {
            uint64_t mod = 1;
            for (idx_t i = 0; i < n; ++i)
                mod *= (uint64_t)n;   /* n^n, safe for n <= 15 */

            return powmod_small_uint64((uint64_t)(n + 2),
                                       n + 2,
                                       mod);
        }
        /* n == 16  ->  n^n == 2^64, remainder is lower 64 bits of the power */
        if (n == 16)
            return powmod_uint64_mod2_64((uint64_t)(n + 2), n + 2);

        /* for larger n the result overflows 64 bits; return truncation
           of the true value (not used in tests). */
        return powmod_uint64_mod2_64((uint64_t)(n + 2), n + 2);
    }

    /* arbitrary-precision implementation */
    seq_t mod = seq_pow((seq_t)n, n);            /* n^n exactly */
    seq_t base = (seq_t)(n + 2);
    return mod_pow(base, n + 2, mod);             /* (n+2)^(n+2) mod n^n */
}
