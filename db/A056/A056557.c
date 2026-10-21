/*
 * A056557  Second tetrahedral coordinate.
 * https://oeis.org/A056557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the second coordinate Y of triples (X,Y,Z) with X≥Y≥Z≥0
 *            sorted lexicographically by X, then Y, then Z. See OEIS A056557
 * Range:     u8 n <= 12, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   binom3, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities and types are provided by the build: */
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "../utils/floor_sqrt.h"
#include "../utils/binom3.h"

seq_t a(idx_t n)
{
    /* Trivial base case: the very first triple is (0,0,0). */
    if (n == 0U) {
        return (seq_t)0;
    }

    /* Find the first coordinate X such that
         binom3(X) > n    (binom3(X) = (X+3 choose 3) = #trials with X' < X) */
    unsigned long long xt = 1ULL;                /* start at 1 to avoid underflow */
    double d_n = (double)n;
    double approx = cbrt(6.0 * d_n);            /* roughly the cube root of 6n */
    xt = (unsigned long long)(approx + 1.0);     /* a safe over‑guess */

    /* Refine the guess.  The loop will take at most a few iterations. */
    while (true) {
        seq_t Bxt = binom3((idx_t)xt);
        if (Bxt > (seq_t)n) {
            /* Bxt is too big; maybe we can step down */
            if (xt == 0ULL) /* should not happen for n>0 */
                break;
            seq_t Bprev = binom3((idx_t)(xt - 1));
            if (Bprev <= (seq_t)n)
                break;                /* we found the correct X = xt */
            else
                xt--;                  /* lower the guess */
        } else {                        /* Bxt <= n, need a larger X */
            xt++;
        }
    }

    /* At this point:  binom3(xt-1) <= n < binom3(xt)  and X = xt. */
    seq_t cumPrev = binom3((idx_t)(xt - 1));      /* cumulative count before block X */
    seq_t d = (seq_t)n - cumPrev;                /* position inside the block */

    /* Compute the second coordinate Y.
       Y is the largest integer with Y(Y+1)/2 <= d.  */
    seq_t tmp = 8 * d + 1;                       /* 8*d + 1 fits in seq_t */
    seq_t sq = floor_sqrt(tmp);                  /* floor(sqrt(8*d+1)) */
    seq_t Y = (sq - 1) / 2;                      /* floor((sqrt(8*d+1)-1)/2) */

    return Y;
}
