/*
 * A089903  Sum of digits of numbers between 0 and (1/9)*(10^n-1).
 * https://oeis.org/A089903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (-88*(-1+10^n)+9*(16+9*10^n)*n)/162
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t pow10n = pow10(n);

    /* Use 128‑bit intermediates for fixed‑width seq_t to avoid overflow. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 p  = (unsigned __int128)pow10n;                     /* 10^n   */
        unsigned __int128 termA =
            (unsigned __int128)9 *
            ((unsigned __int128)16 + (unsigned __int128)9 * p) *
            (unsigned __int128)n;                                            /* 9*(16+9·10^n)*n */
        unsigned __int128 termB = (unsigned __int128)88 * (p - 1);           /* 88*(10^n-1) */
        unsigned __int128 numer = termA - termB;                            /* numerator */
        return (seq_t)(numer / (unsigned __int128)162);                     /* division by 162 */
    } else {
        seq_t termA = (seq_t)9 * ((seq_t)16 + (seq_t)9 * pow10n) * (seq_t)n;
        seq_t termB = (seq_t)88 * (pow10n - (seq_t)1);
        seq_t numer = termA - termB;
        return numer / (seq_t)162;
    }
}
