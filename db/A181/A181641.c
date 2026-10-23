/*
 * A181641  Expansion of sqrt(1-4*x)/(1+x).
 * https://oeis.org/A181641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 1, a(1) = -3 For n ≥ 2: n*a(n) + 3*(2‑n)*a(n‑1) +
 *            2*(3‑2*n)*a(n‑2) = 0 i.e. a(n) = -( 3*(2‑n)*a(n‑1) +
 *            2*(3‑2*n)*a(n‑2) ) / n
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 17, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;   /* cache of computed terms */
    static idx_t size = 0;      /* current cache size (next free index) */

    /* Return cached value if already present */
    if (n < size)
        return seq[n];

    /* Grow cache to hold term n */
    idx_t old_size = size;
    seq = (seq_t *)realloc(seq, (n + 1) * sizeof(seq_t));
    if (!seq)
        return 0;   /* allocation failed – return zero */
    size = n + 1;

    /* Initialise base cases on first allocation */
    if (old_size == 0)            /* no previous allocation */
        seq[0] = (seq_t)1;
    if (size >= 2 && old_size < 2) /* ensure a(1) is set when it is needed */
        seq[1] = (seq_t)-3;

    /* Compute missing terms using the recurrence (n ≥ 2) */
    for (idx_t i = (old_size > 1 ? old_size : 2); i <= n; ++i)
    {
        seq_t t1 = (seq_t)3 * ((seq_t)2 - (seq_t)i) * seq[i - 1];
        seq_t t2 = (seq_t)2 * ((seq_t)3 - (seq_t)2 * (seq_t)i) * seq[i - 2];
        seq[i] = -(t1 + t2) / (seq_t)i;   /* division is exact by the recurrence */
    }

    return seq[n];
}
