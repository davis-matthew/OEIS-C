/*
 * A128368  a(n) = least k such that the remainder when 28^k is divided by k
 *          is n.
 * https://oeis.org/A128368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, nonn
 * Formula:   a(n) = least k such that 28^k ≡ n (mod k)
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following utility converts a sequence element “base” to the
 * k‑th power modulo “mod”. */
#include "../utils/mod_pow.h"
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n + (seq_t)1;          /* start searching from n+1 */
    for (;; ++k)                           /* k will always stay < 2^64 */
    {
        seq_t rem = mod_pow((seq_t)28, (uint64_t)k, k);
        if (rem == (seq_t)n)
            return k;
    }
}
