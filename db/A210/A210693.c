/*
 * A210693  Smallest positive integer k (different from n) such that n^k mod
 *          (2*k+1) = k^n mod (2*n+1).
 * https://oeis.org/A210693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer k ≠ n such that * n^k (mod
 *            2k+1) = k^n (mod 2n+1)
 * Range:     u8 n <= 26, u16 n <= 1642, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t base, idx_t exp, seq_t mod) */

seq_t a(idx_t n)
{
    /* n is 1‑based index (≥1) */
    for (seq_t k = 1; ; ++k) {
        if (k == n)                /* k must differ from n */
            continue;

        /* Moduli for the two congruences */
        seq_t mod1 = 2 * k + 1;                        /* 2*k+1 */
        seq_t mod2 = 2 * (seq_t)n + 1;                 /* 2*n+1 */

        /* r1 = n^k mod (2*k+1)  (use modular exponentiation) */
        seq_t r1 = mod_pow((seq_t)n, (idx_t)k, mod1);

        /* r2 = k^n mod (2*n+1) */
        seq_t r2 = mod_pow(k, (idx_t)n, mod2);

        if (r1 == r2)
            return k;       /* smallest k found */
    }
}
