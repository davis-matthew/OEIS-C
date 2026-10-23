/*
 * A197974  Ceiling((n+1/5)^n).
 * https://oeis.org/A197974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((n + 1/5)^n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* a(1) corresponds to the first term.  Return 0 for n==0 to keep
       behaviour defined for all idx_t values. */
    if (n == 0) return (seq_t)0;

    /* When seq_t is small (unsigned 8‑byte or less) we use unsigned __int128
       for the intermediate arithmetic.  This safely evaluates the expression
       for all n whose result fits in seq_t.  For larger seq_t values we use
       the pow_seq routine which operates on arbitrary‑precision types. */
    if (sizeof(seq_t) <= 8) {
        /* Use unsigned __int128 for intermediate values when available. */
        unsigned __int128 base = (unsigned __int128)5 * (unsigned __int128)n + (unsigned __int128)1;
        unsigned __int128 num = (unsigned __int128)1;
        unsigned __int128 den = (unsigned __int128)1;

        /* Compute (5*n+1)^n and 5^n with 128‑bit integers. */
        for (idx_t i = 0; i < n; ++i) {
            num *= base;
            den *= (unsigned __int128)5;
        }

        /* ceil division: (num + den - 1) / den. */
        unsigned __int128 result128 = (num + den - (unsigned __int128)1) / den;
        return (seq_t)result128;
    } else {
        /* seq_t is large (e.g., arbitrary‑precision).  Use pow_seq. */
        seq_t base = (seq_t)5 * (seq_t)n + (seq_t)1;
        seq_t num = pow_seq(base, (int)n);
        seq_t den = pow_seq((seq_t)5, (int)n);
        seq_t result = (num + den - (seq_t)1) / den;
        return result;
    }
}
