/*
 * A041122  Numerators of continued fraction convergents to sqrt(70).
 * https://oeis.org/A041122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 502*a(n-6) - a(n-12) (for n ≥ 12) a(0) = 8, a(1) = 17,
 *            a(2) = 25, a(3) = 67, a(4) = 92, a(5) = 251, a(6) = 4108, a(7) =
 *            8467, a(8) = 12575, a(9) = 33617, a(10) = 46192, a(11) = 126001
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Initial 12 terms of the sequence */
static const seq_t _a_init[12] = {
    (seq_t)8,   (seq_t)17,  (seq_t)25,  (seq_t)67,
    (seq_t)92,  (seq_t)251, (seq_t)4108, (seq_t)8467,
    (seq_t)12575, (seq_t)33617, (seq_t)46192, (seq_t)126001
};

seq_t a(idx_t n)
{
    /* Direct return for the first 12 indices */
    if (n < 12u)
        return _a_init[(size_t)n];

    /* Allocate a buffer large enough for all terms up to n */
    size_t len = (size_t)n + 1;
    seq_t *buf = (seq_t*)malloc(len * sizeof(seq_t));
    if (buf == NULL)
        abort();        /* allocation failed */

    /* Initialise the first 12 values */
    for (size_t i = 0; i < 12; ++i)
        buf[i] = _a_init[i];

    /* Compute remaining terms using the recurrence */
    for (idx_t i = 12; i <= n; ++i)
        buf[i] = 502 * buf[i - 6] - buf[i - 12];

    seq_t result = buf[n];
    free(buf);
    return result;
}
