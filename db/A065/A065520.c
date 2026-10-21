/*
 * A065520  Numbers n with the property that if m is formed from n by dropping
 *          any number (possibly zero) of initial or final digits then there
 *          is a prime ending with m.
 * https://oeis.org/A065520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = the n-th integer (in ascending order) whose decimal
 *            representation starts with a digit in {1,2,3,5,7,9} and all
 *            following digits are in {1,3,7,9}. This is equivalent to
 *            generating the sequence of numbers satisfying the OEIS A065520
 *            property
 * Range:     u8 n <= 54, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t nseq = (seq_t)n;

    /* Find the digit length d of a(n). */
    seq_t cumulative = 0;         /* total count of numbers with < d digits */
    seq_t pow = 1;                /* 4^(d-1) for current d */
    int d = 0;

    while (1) {
        ++d;
        seq_t count = (seq_t)6 * pow;     /* count of d‑digit numbers */
        if (cumulative + count >= nseq) {
            break;
        }
        cumulative += count;
        pow *= (seq_t)4;                 /* prepare 4^d for next iteration */
    }

    seq_t pow_start = pow;           /* 4^(d-1) for the found d */
    seq_t offset   = nseq - cumulative - 1; /* 0‑based rank within d‑digit block */

    /* digit tables in increasing order */
    static const int firstDigits[6] = {1, 2, 3, 5, 7, 9};
    static const int laterDigits[4] = {1, 3, 7, 9};

    /* Extract first digit */
    int fidx = (int)(offset / pow_start);
    offset  = offset % pow_start;
    seq_t val = (seq_t)firstDigits[fidx];

    /* Extract remaining digits */
    for (int pos = 2; pos <= d; ++pos) {
        pow_start /= (seq_t)4;               /* 4^(d-pos) */
        int didx = (int)(offset / pow_start);
        offset  = offset % pow_start;
        val = val * 10 + (seq_t)laterDigits[didx];
    }

    return val;
}
