/*
 * A011525  Stirling numbers of first kind S1(15,n).
 * https://oeis.org/A011525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, fini, full
 * Formula:   not stated
 * Range:     i8 none, i16 none, i32 none, i64 n <= 15, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the signed Stirling numbers of the first kind for n = 15.
   a(n) = s(15,n) for 1 ≤ n ≤ 15, and a(n) = 0 for n > 15.
   Recurrence (for all k):
        s(0,0) = 1,  s(n,0) = 0  (n>0)
        s(n,k) = s(n-1,k-1) - (n-1)*s(n-1,k)   (k>0)            */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* One‑time precomputation of the 15th row of the signed Stirling triangle. */
    static bool inited = false;
    static seq_t s[16];                    /* indices 0..15; we use 1..15 */

    if (!inited) {
        /* s[0] = 1 for the row 0, all other entries are 0. */
        s[0] = (seq_t)1;
        for (idx_t i = 1; i <= 15; ++i) {
            /* For n > 0 the entry s[n,0] must be 0.  It holds from the
               previous iteration because we set it to 0 after i==1. */
            if (i > 1)
                s[0] = (seq_t)0;

            for (idx_t j = i; j > 0; --j) {
                /* s(i,j) = s(i-1,j-1) – (i-1)*s(i-1,j)   */
                s[j] = s[j-1] - (seq_t)(i-1) * s[j];
            }
        }
        inited = true;
    }

    return (n >= 1 && n <= 15) ? s[(idx_t)n] : (seq_t)0;
}
