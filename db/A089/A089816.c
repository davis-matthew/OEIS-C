/*
 * A089816  a(n) = floor((n+3)^(n+2)/((n+3)^2-1)).
 * https://oeis.org/A089816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+3)^(n+2)/((n+3)^2-1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute M = n + 3 as a seq_t value */
    seq_t M = (seq_t)n + 3;

    /* If seq_t is at most 64 bits, use 128‑bit arithmetic
     * to avoid overflow of intermediate values. */
    if (sizeof(seq_t) <= sizeof(unsigned long long))
    {
        unsigned __int128 base = (unsigned __int128)M;
        unsigned __int128 num   = 1;
        idx_t        exp = n + 2;

        /* binary exponentiation using 128‑bit intermediates */
        while (exp)
        {
            if (exp & 1u) num *= base;
            base *= base;
            exp >>= 1;
        }

        /* denominator (M^2-1) also fits in 128 bits for all n of interest */
        unsigned __int128 den = (unsigned __int128)M * (unsigned __int128)M - 1;

        /* floor division for positive numbers */
        seq_t result = (seq_t)(num / den);
        return result;
    }
    else
    {
        /* For arbitrary‑precision seq_t types, use the provided big‑int
         * exponentiation routine. */
        seq_t num = seq_pow(M, n + 2);
        seq_t den = M * M - 1;
        return num / den;
    }
}
