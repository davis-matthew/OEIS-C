/*
 * A059774  Consider the line segment in R^n from the origin to the point
 *          P=(1,2,3,...,n); let d = squared distance to this line from the
 *          closest point of Z^n (excluding the endpoints). Sequence gives d
 *          times P.P.
 * https://oeis.org/A059774
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   sum_of_integers, sum_of_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A059774:
   a(n) = d * P.P where d is squared distance from a lattice point
   (excluding 0 and P) to the line segment from origin to P=(1,2,…,n),
   and P.P = Σ_{k=1}^n k².  The distance squared of an integer vector Q
   is |Q|² − (Q·P)² / |P|², so a(n) is
        a(n) = |P|² |Q|² − (Q·P)²
   minimized over all integer vectors Q ≠ 0, P.  For 0 ≤ t < n let
   Q consist of the last (n−t) coordinates equal to 1 and the first
   t coordinates equal to 0.  Then
        f(t) = |P|² (n−t) − (S(n)−S(t))²
   where S(m)=Σ_{k=1}^m k.  The minimal positive value of f(t) is the
   required a(n).  For n ≥ 10 the minimiser is t=1 and a(n)=A000330(n‑1)
   (i.e. Σ_{k=1}^{n‑1}k²).  The following implementation works for all
   n ≥ 2 and is O(n). */

#include <stdint.h>
#include "../utils/sum_of_integers.h"
#include "../utils/sum_of_squares.h"

seq_t a(idx_t n)
{
    /* n must be at least 2 */
    if (n < 2) return 0;

    /* For n ≥ 10 the result is known to be sum_of_squares(n-1) */
    if (n >= 10) return sum_of_squares(n - 1);

    seq_t S   = sum_of_integers(n);
    seq_t S2  = sum_of_squares(n);
    /* Initialise best to a value that is larger than any possible f(t) */
    seq_t best = S2 * n;                   /* upper bound for a(n) */

    for (idx_t t = 0; t < n; ++t) {
        seq_t St   = sum_of_integers(t);
        seq_t ones = n - t;
        seq_t diff = S - St;
        seq_t diff_sq = diff * diff;
        seq_t prod = S2 * ones;

        /* Avoid unsigned underflow – only consider non‑negative values */
        if (prod < diff_sq) continue;

        seq_t val = prod - diff_sq;
        if (val < best) best = val;
    }
    return best;
}
