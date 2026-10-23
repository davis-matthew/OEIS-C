/*
 * A104105  a(1) = 1, if A(k) = sequence of first 2^k -1 terms and if B(k) is
 *          A(k) with 0's and 1's exchanged, then A(k+1) = A(k),1,B(k) if a(k)
 *          = 0, A(k+1) = A(k),0,B(k) if a(k) = 1.
 * https://oeis.org/A104105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence definition (OEIS A104105):
   a(1) = 1;
   For n > 1 let p = 2^⌊log₂ n⌋ (the largest power of 2 ≤ n).
   • If n == p :   a(n) = 1 – a(log₂ n)
   • Otherwise   : a(n) = 1 – a(n – p)
   The following implementation follows this recurrence. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t m = n;            /* work on a copy of the index       */
    bool inv = false;       /* pending complement flag           */

    while (m > 1) {
        /* test whether m is a power of two: m & (m-1) == 0   */
        if ((m & (m - 1)) == 0) {                /* m == 2^k */
            unsigned long long u = (unsigned long long)m;
            /* compute k = log₂(m)  (most significant bit position)   */
            int k = 63 - __builtin_clzll(u);
            m = (idx_t)k;                /* recurse on k          */
            inv = !inv;                  /* a(m) = 1 – a(k)       */
        } else {                               /* m in the B(k) part    */
            unsigned long long u = (unsigned long long)m;
            int k = 63 - __builtin_clzll(u);   /* ⌊log₂(m)⌋            */
            idx_t p = ((idx_t)1) << k;        /* p = 2^k              */
            m -= p;                           /* n - p                */
            inv = !inv;                       /* complement            */
        }
    }
    /* m == 1 -> a(1) = 1, apply pending complements if any */
    return inv ? (seq_t)0 : (seq_t)1;
}
