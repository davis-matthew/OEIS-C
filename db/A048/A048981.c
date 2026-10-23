/*
 * A048981  Squarefree values of n for which the quadratic field Q[ sqrt(n) ]
 *          is norm-Euclidean.
 * https://oeis.org/A048981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, sign, full, nice
 * Formula:   not stated
 * Range:     i8 n <= 21, i16 n <= 21, i32 n <= 21, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: squarefree values n for which Q[ sqrt(n) ] is norm‑Euclidean.
   The discriminants of all norm‑Euclidean quadratic fields are known
   (classical theorem).  There are exactly 21 such discriminants:
   -11, -7, -3, -2, -1, 2, 3, 5, 6, 7, 11, 13, 17, 19, 21, 29, 33, 37,
   41, 57, 73.  The sequence is finite; values beyond index 21 are
   undefined, so the function returns 0 for such indices. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Array containing the 21 values in ascending order. */
    static const seq_t vals[] = {
        -11, -7, -3, -2, -1,
        2, 3, 5, 6, 7,
        11, 13, 17, 19, 21,
        29, 33, 37, 41, 57,
        73
    };
    /* Number of terms in the sequence (compile‑time constant). */
    static const idx_t len = sizeof(vals) / sizeof(vals[0]);

    /* Index n is 1‑based; return 0 if out of range. */
    if (n < 1 || n > len)
        return (seq_t)0;

    return vals[(idx_t)(n - 1)];
}
