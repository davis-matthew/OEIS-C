/*
 * A070191  Numbers k such that gcd(3*k, 8^k+1) = 3.
 * https://oeis.org/A070191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 6697, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th integer k such that gcd(3*k, 8^k + 1) == 3
 * (OEIS A070191).
 */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

/* Return the n-th term of OEIS sequence A070191. */
seq_t a(idx_t n)
{
    if (n == 0) /* under‑offset behaviour is irrelevant */
        return (seq_t)0;

    idx_t found = 0;
    for (idx_t k = 1; found < n; ++k)
    {
        seq_t mod  = (seq_t)3 * (seq_t)k;      /* 3*k as seq_t */
        seq_t rem  = mod_pow((seq_t)8, k, mod);/* 8^k mod (3*k) */
        seq_t tmp  = rem + (seq_t)1;
        if (tmp >= mod)                       /* ensure (8^k + 1) mod (3*k) */
            tmp -= mod;
        seq_t g = gcd(mod, tmp);              /* gcd(3*k, 8^k+1) */
        if (g == (seq_t)3) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
    }

    /* unreachable for valid n >= 1 */
    return (seq_t)0;
}
