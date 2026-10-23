/*
 * A130527  A permutation of the integers induced by the Beatty sequence for
 *          sqrt(2).
 * https://oeis.org/A130527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=0; For n>0, let L(k)=⌊k·√2⌋ and U(k)=2k+L(k). Then a(n)= k
 *            if n=L(k), a(n)=−k if n=U(k)
 * Range:     i8 n <= 7, i16 n <= 67, i32 n <= 67, i64 n <= 67, i128 n <= 67,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the term a(n) for any n≥0.  It relies only
   on integer arithmetic and the helper floor_sqrt for computing
   floor(k·√2) as floor_sqrt(2·k²).  No additional global state is
   required, so a(n) can be called in arbitrary order. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* iterate over successive k until both L(k) and U(k) exceed n */
    for (idx_t k = 1;; ++k) {
        seq_t kk   = (seq_t)k;
        seq_t Lk   = floor_sqrt(kk * kk * (seq_t)2);  /* floor(k·√2) */
        if (Lk == (seq_t)n)
            return kk;                                /* a(L(k)) = k */

        seq_t Uk = (seq_t)2 * kk + Lk;                 /* U(k) = 2k + L(k) */
        if (Uk == (seq_t)n)
            return -kk;                               /* a(U(k)) = –k */

        if (Lk > (seq_t)n && Uk > (seq_t)n)
            break; /* all indices > n have been processed */
    }

    /* In the mathematically correct definition this point is never
       reached.  The following return satisfies the compiler about
       a missing return statement. */
    return (seq_t)0;
}
