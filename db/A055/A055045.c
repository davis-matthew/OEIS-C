/*
 * A055045  Numbers of the form 4^i*(8*j+5).
 * https://oeis.org/A055045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 8922, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers of the form 4^i * (8*j+5) */
#include <stddef.h>
#include <stdint.h>

#define MAX_I 64  /* maximum i considered – safe even for n up to millions */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not used but defined for completeness */

    /* Estimate how many progressions are needed.
       The nth term is roughly 6*n, so go far enough so that
       the smallest base 5*4^i is <= 6*n+100. */
    idx_t limit = 6 * n + 100;    /* safe margin */
    seq_t pow4 = 1;
    size_t i_max = 0;
    while ((seq_t)5 * pow4 <= (seq_t)limit && i_max < MAX_I) {
        i_max++;
        pow4 *= 4;
    }
    if (i_max == 0) i_max = 1;   /* ensure at least one progression */

    /* Initialise current values and steps for each progression i=0..i_max-1. */
    seq_t cur[MAX_I];
    seq_t step[MAX_I];
    pow4 = 1;
    for (size_t i = 0; i < i_max; ++i) {
        cur[i]  = (seq_t)5 * pow4;
        step[i] = (seq_t)8 * pow4;
        pow4 *= 4;
    }

    seq_t value = 0;
    /* Merge the progression values by repeatedly extracting the smallest. */
    for (idx_t pos = 1; pos <= n; ++pos) {
        size_t mi = 0;
        for (size_t i = 1; i < i_max; ++i)
            if (cur[i] < cur[mi]) mi = i;
        value = cur[mi];
        cur[mi] += step[mi];
    }
    return value;
}
