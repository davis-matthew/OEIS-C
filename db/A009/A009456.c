/*
 * A009456  Expansion of sin(log(1+x))*cosh(x).
 * https://oeis.org/A009456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A009456.
   a(n) = n! * [x^n] sin(log(1+x))*cosh(x)
   The coefficients can be computed by
       a(n) = Σ_{k ≡ n (mod 2)}  Q_k · C(n,k)
   where Q_k is the imaginary part of the falling factorial (i)_k,
   i.e. (i)_0 = 1, (i)_{k+1} = (i - k)·(i)_k.
   Q_k is obtained from the recurrence
       P_0 = 1 ,  Q_0 = 0
       P_{k+1} = -k·P_k - Q_k
       Q_{k+1} =  P_k - k·Q_k
   (P_k, Q_k are the real and imaginary parts of (i)_k).  The binomial
   coefficient is C(n,k) = n!/(k!(n-k)!).
   This implementation is O(n) per call and uses only integer arithmetic. */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t parity = n & 1u;            /* parity of n (0=even, 1=odd) */
    seq_t res   = 0;                  /* accumulator for a(n)   */
    seq_t binom = 1;                  /* C(n,0)                 */
    seq_t P     = 1;                  /* Re{(i)_0}              */
    seq_t Q     = 0;                  /* Im{(i)_0}              */

    for (idx_t k = 0; k <= n; ++k) {
        /* Q currently holds Im{(i)_k} */
        if ((k & 1u) == parity) {
            res += Q * binom;
        }

        if (k == n) break;              /* last term processed */

        /* update binomial coefficient C(n,k+1) from C(n,k) */
        binom = binom * (seq_t)(n - k) / (seq_t)(k + 1u);

        /* advance the falling factorial (i)_k --> (i)_{k+1} */
        seq_t Pnext = - (seq_t)k * P - Q;
        seq_t Qnext =   P - (seq_t)k * Q;
        P = Pnext;
        Q = Qnext;
    }

    return res;
}
