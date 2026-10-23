/*
 * A134186  Expansion of -x*(5*x^4+5*x^3-7*x-4)/((x^2-x-1)*(x^3+x^2-1)).
 * https://oeis.org/A134186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(1)=4, a(2)=3, a(3)=5, a(4)=4, a(5)=7 Recurrence (for n ≥ 6):
 *            a(n) = -a(n‑1) + 2·a(n‑2) + 2·a(n‑3) – a(n‑5)
 * Range:     i8 n <= 14, i16 n <= 27, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t size = 0;

    if (n < 1) return (seq_t)0;            /* undefined for n < 1 */

    if (size == 0) {                       /* initialise first 5 terms */
        size = 5;
        seq = (seq_t *)malloc((size_t)size * sizeof(seq_t));
        if (!seq) return (seq_t)0;
        seq[0] = 4;
        seq[1] = 3;
        seq[2] = 5;
        seq[3] = 4;
        seq[4] = 7;
    }

    if (n > size) {                        /* extend cache up to n   */
        seq_t *tmp = (seq_t *)realloc(seq,
                                      (size_t)n * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        seq = tmp;
        for (idx_t i = size + 1; i <= n; ++i)  /* i → a(i)              */
        {
            /* a(i) = -a(i-1) + 2*a(i-2) + 2*a(i-3) - a(i-5) */
            seq[i-1] = -seq[i-2] + 2*seq[i-3] + 2*seq[i-4]
                        - seq[i-6];
        }
        size = n;
    }

    return seq[n-1];
}
