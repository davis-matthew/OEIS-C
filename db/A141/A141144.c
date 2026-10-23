/*
 * A141144  X values of the complete set of 23 integer solutions to the Ochoa
 *          curve equation.
 * https://oeis.org/A141144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, sign
 * Formula:   not stated
 * Range:     i8 none, i16 n <= 20, i32 n <= 23, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   isqrt_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* size_t */
#include "../utils/isqrt_ll.h"

/* Computes the integer X‑coordinates of all solutions of the Ochoa curve
   3Y^2 = 2X^3 + 386X^2 + 256X – 58195.  The function scans X in a
   safe interval containing all 23 solutions, records the ones that give a
   perfect square, and memoizes the results for subsequent calls. */

static seq_t  solutions[100];
static size_t solutions_cnt = 0;

/* Find all integer X‑values once, ahead of time. */
static void init(void)
{
    if (solutions_cnt) return;  /* already initialized */

    /* Search range chosen to be comfortably larger than the largest known
       solution (1 657 891).  The curve has only 23 integer solutions. */
    for (long long x = -2000000LL; x <= 2000000LL; ++x) {
        long long cube = x * x * x;
        long long rhs  = cube - 440067LL * x + 106074110LL;
        if (rhs < 0) continue;           /* cannot be a perfect square */

        long long s = isqrt_ll(rhs);
        if (s * s == rhs) {
            solutions[solutions_cnt++] = (seq_t)x;
        }
    }
}

/* External sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; behaviour below offset is irrelevant */

    init();  /* ensure all solutions are precomputed */

    if (n <= solutions_cnt)
        return solutions[(size_t)(n - 1)];

    return 0;  /* return 0 for indices beyond the known solutions */
}
