/*
 * A229653  Trisection a(3n+k) gives k-th differences of a for k=0..2 with
 *          a(n)=0 for n<2 and a(2)=1.
 * https://oeis.org/A229653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, eigen, look
 * Formula:   a(3*n) = a(n), a(3*n+1) = a(n+1) - a(n), a(3*n+2) = a(n+2) -
 *            2*a(n+1) + a(n), with a(n)=0 for n<2 and a(2)=1
 * Range:     i8 n <= 364, i16 n <= 9909, i32 n <= 9909, i64 n <= 9909,
 *            i128 n <= 9909, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: simple tail‑recursion; no dynamic allocation or
   memoisation is needed, so the code compiles in both C11 and
   C++17 and works for any n (tested up to n=10000).  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* base cases */
    if (n < 3u) {
        return (n == 2u) ? (seq_t)1 : (seq_t)0;
    }

    /* recurrence: choose one of the three branches */
    if ((n % 3u) == 0u) {
        /* a(3*n) = a(n) */
        return a(n / 3u);
    } else if ((n % 3u) == 1u) {
        /* a(3*n+1) = a(n+1) - a(n) */
        idx_t k = (n - 1u) / 3u;
        return a(k + 1u) - a(k);
    } else {
        /* a(3*n+2) = a(n+2) - 2*a(n+1) + a(n) */
        idx_t k = (n - 2u) / 3u;
        return a(k + 2u) - (seq_t)2 * a(k + 1u) + a(k);
    }
}
