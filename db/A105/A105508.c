/*
 * A105508  Numbers m such that 8 is the leading digit of the m-th Fibonacci
 *          number in decimal representation.
 * https://oeis.org/A105508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth index m such that the leading decimal digit of the
 *            m-th Fibonacci number equals 8
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Precomputed constants needed to determine the leading digit
   of Fib(m) without constructing the number itself. */
static const double LOG10_PHI = 0.20898764024997873;
static const double LOG10_SQRT5 = 0.3494850021680094;

/* The function is thread‑safe only if it is called from a single
   thread; for multi‑threaded use one can add a mutex around the
   static data. */
seq_t a(idx_t n)
{
    /* Static data to remember the list of found indices
       and the last Fibonacci index checked. */
    static idx_t     last_m     = 0;      /* last m examined */
    static idx_t     found      = 0;      /* number of indices stored */
    static idx_t     cap        = 0;      /* array capacity           */
    static seq_t   * indices   = NULL;    /* array of found indices  */

    /* Initial allocation. */
    if (indices == NULL) {
        cap = 128;
        indices = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    /* Continue scanning Fibonacci indices until we have
       at least n entries. */
    while (found < n) {
        ++last_m;

        /* Compute the fractional part of log10(Fib(last_m))
           using the closed formula: Fib(m) ≈ phi^m / sqrt(5). */
        double x  = (double)last_m * LOG10_PHI - LOG10_SQRT5;
        double frac = x - floor(x);
        double d   = pow(10.0, frac);

        /* Determine the leading decimal digit. */
        int leading = (int)floor(d + 1e-12);   /* tiny epsilon to guard
                                                  against rounding */
        if (leading == 8) {
            /* Grow the array if necessary. */
            if (found == cap) {
                cap <<= 1;
                indices = (seq_t*)realloc(indices, cap * sizeof(seq_t));
            }
            indices[found++] = (seq_t)last_m;
        }
    }

    return indices[n - 1];   /* a(1) corresponds to indices[0] */
}
