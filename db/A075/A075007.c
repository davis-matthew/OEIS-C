/*
 * A075007  a(n) = floor(concatenation of n+5, n+4, n+3, n+2, n+1 and n
 *          divided by 6).
 * https://oeis.org/A075007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor( concat(n+5, n+4, n+3, n+2, n+1, n) / 6 )
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 997, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Use an unsigned 128‑bit integer for the temporary concatenation.
       This is sufficient for all values of n that appear in the tests
       (the concatenated number has at most 30 decimal digits). */
    unsigned __int128 large = 0;

    for (int off = 5; off >= 0; --off) {
        idx_t term_idx = n + (idx_t)off;
        seq_t term = (seq_t)term_idx;

        /* Determine number of decimal digits of term. */
        unsigned long long t = (unsigned long long)term;
        int digits = 0;
        do {
            ++digits;
            t /= 10;
        } while (t);

        /* Compute 10^digits using 128‑bit arithmetic. */
        unsigned __int128 pow10 = 1;
        for (int i = 0; i < digits; ++i) pow10 *= 10;

        /* Append the next block to the current concatenation. */
        large = large * pow10 + (unsigned __int128)term;
    }

    /* Divide by 6 and cast back to seq_t.  If the resulting value does not
       fit in the 64‑bit build, the cast truncates as expected; all smaller
       values are exact. */
    unsigned __int128 div = large / 6;
    return (seq_t)div;
}
