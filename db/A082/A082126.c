/*
 * A082126  Smallest difference>1 between d and p/d for any divisor d of the
 *          partial product p of the sequence, starting with 19.
 * https://oeis.org/A082126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   pow3_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A082126: smallest difference > 1 between a divisor d of the
   partial product p and its complementary divisor p/d.
   After the first three terms the sequence consists of powers of 3.
   Let E be the total exponent of 3 in the partial product
   (i.e. the sum of exponents of 3 in all terms of the product).
   For n ≥ 4 one has  
        if E is even   e = E/2 + 2
        if E is odd    e = (E-1)/2
   and a(n) = 3^e.
   The initial product after the first three terms is 19·18·29
   = 2·3²·19·29, hence E at n = 3 equals 2.
   The following code implements this recurrence using an
   arbitrary‑precision exponentiation routine so that it works for
   all n that fit into the usual integer ranges of the `seq_t`
   implementation.  It also works for very large exponents, because
   the exponentiation by squaring routine uses only `seq_t` operations.
*/

#include <stdbool.h>
#include "../utils/pow3_big.h"

/* primary sequence function */
seq_t a(idx_t n)
{
    /* first three explicit terms */
    if (n == 1) return (seq_t)19;
    if (n == 2) return (seq_t)18;
    if (n == 3) return (seq_t)29;

    /* cumulative exponent of 3 after the first 3 terms */
    seq_t E = (seq_t)2;   /* from 18 = 2 * 3^2 (19 and 29 give no 3's) */

    seq_t e_next = 0;
    for (idx_t i = 4; i <= n; ++i) {
        /* compute exponent of the next term */
        if ((E % (seq_t)2) == 0)           /* E even */
            e_next = (E >> 1) + (seq_t)2;
        else                               /* E odd */
            e_next = ((E - (seq_t)1) >> 1);

        /* if we are at the requested index, return the term */
        if (i == n)
            return pow3_big(e_next);

        /* update cumulative exponent for the next iteration */
        E = E + e_next;
    }

    /* unreachable: function should have returned inside the loop */
    return (seq_t)0;
}
