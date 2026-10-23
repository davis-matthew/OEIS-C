/*
 * A040591  Continued fraction for sqrt(616).
 * https://oeis.org/A040591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = n-th term of the simple continued fraction expansion of
 *            sqrt(616). The algorithm computes the integer part a0 =
 *            floor(sqrt(616)) and generates the periodic sequence using the
 *            standard recurrence until the term 2*a0 is reached
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* The constant whose square root we expand */
    const seq_t D = (seq_t)616;

    /* Integer part a0 = floor(sqrt(D)) */
    seq_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D)
        a0++;

    /* Lazily compute the repeating pattern once */
    static seq_t *pat = NULL;      /* array of periodic terms (excluding a0) */
    static size_t period = 0;
    static int initialized = 0;

    if (!initialized) {
        initialized = 1;

        /* Initial values for the continued fraction algorithm */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a_term = a0;

        /* Grow the pattern array as we discover new terms */
        size_t cap = 0;
        for (;;) {
            /* Prepare for next term */
            m = d * a_term - m;
            d = (D - m * m) / d;
            a_term = (a0 + m) / d;

            /* Grow array if necessary */
            if (period == cap) {
                size_t new_cap = cap == 0 ? 8 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(pat, new_cap * sizeof(seq_t));
                if (!tmp) abort();  /* out of memory – terminate */
                pat = tmp;
                cap = new_cap;
            }

            pat[period++] = a_term;

            /* Period ends when the term equals 2*a0 */
            if (a_term == a0 * 2)
                break;
        }
    }

    if (n == 0)
        return a0;

    /* Return term from the periodic pattern */
    idx_t idx = (n - 1) % period;
    return pat[idx];
}
