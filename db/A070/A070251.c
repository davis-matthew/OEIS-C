/*
 * A070251  Unrelated-factorial numbers: product of numbers unrelated to n
 *          (numbers which have a common divisor with n but do not divide n).
 * https://oeis.org/A070251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all k < n such that 1 < gcd(k,n) < k
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 23, u64 n <= 35, u128 n <= 53,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent definition: a(n) = product_{k<n, 1<gcd(k,n)<k} k */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    /* n may be up to 64‑bit; cast to native integer for modulo */
    uint64_t un = (uint64_t)n;

    for (idx_t k = 2; k < n; ++k) {
        /* compute gcd(k,n); result fits in seq_t */
        seq_t g = gcd((seq_t)k, (seq_t)n);
        if (g > 1) {
            /* skip k that divides n */
            if (un % (uint64_t)k != 0) {
                result *= (seq_t)k;
            }
        }
    }

    return result;
}
