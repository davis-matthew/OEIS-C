/*
 * A183555  Positions of the records of the positive integers in A179319; a(n)
 *          is the first position in A179319 equal to +n.
 * https://oeis.org/A183555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A183555:
   a(n) = A059973(4*n+2) - 2 for even n
   a(n) = A059973(4*((n+1)/2)+1) - 2 for odd n >1
   a(1) = 0
   A059973 is defined by the linear recurrence:
        a_0 = 0, a_1 = 1, a_2 = 1, a_3 = 2
        a_k = 4*a_{k-2} + a_{k-4}   for k >= 4
   The following implementation evaluates the recurrence
   using only a fixed array of four elements. */

#include <stdint.h>

static seq_t A059973_Rec(idx_t k) {
    if (k == 0) return (seq_t)0;
    if (k == 1) return (seq_t)1;
    if (k == 2) return (seq_t)1;
    if (k == 3) return (seq_t)2;
    seq_t prev[4];
    prev[0] = (seq_t)0; /* a_0 */
    prev[1] = (seq_t)1; /* a_1 */
    prev[2] = (seq_t)1; /* a_2 */
    prev[3] = (seq_t)2; /* a_3 */
    for (idx_t i = 4; i <= k; ++i) {
        seq_t term4 = prev[(i - 2) & 3] * (seq_t)4;
        seq_t term1 = prev[(i - 4) & 3];
        prev[i & 3] = term4 + term1;
    }
    return prev[k & 3];
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;     /* undefined for n < 1, but safe */
    if (n == 1) return (seq_t)0;    /* special case */
    if (n & 1) { /* odd n > 1 */
        idx_t m = (n + 1) >> 1;     /* (n+1)/2 */
        idx_t k = 4 * m + 1;
        seq_t val = A059973_Rec(k);
        return val - (seq_t)2;
    } else { /* even n */
        idx_t m = n >> 1;           /* n/2 */
        idx_t k = 4 * m + 2;
        seq_t val = A059973_Rec(k);
        return val - (seq_t)2;
    }
}
