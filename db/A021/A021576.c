/*
 * A021576  Decimal expansion of 1/572.
 * https://oeis.org/A021576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021576 – decimal expansion of 1/572
   Recurrence: a(n) = a(n-1) - a(n-3) + a(n-4)  for n > 5
   Initial terms: a(0)=0, a(1)=0, a(2)=1, a(3)=7, a(4)=4, a(5)=8
   After the first two zeros the sequence is periodic with period 6:  
   {1, 7, 4, 8, 2, 5} */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* For n < 2 the two leading zeros. */
    if (n < 2) {
        return (seq_t)0;
    }

    /* The cycle repeats every 6 terms starting at n = 2. */
    static const seq_t cycle[6] = {1, 7, 4, 8, 2, 5};

    /* (n - 2) % 6 selects the correct element of the cycle. */
    idx_t idx = (n - 2) % 6;
    return cycle[(size_t)idx];
}
