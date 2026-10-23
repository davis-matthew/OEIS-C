/*
 * A008671  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^7)).
 * https://oeis.org/A008671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-3)-a(n-5)+a(n-7)-a(n-9)-a(n-10)+a(n-12)
 *            a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=1, a(5)=1, a(6)=2, a(7)=2,
 *            a(8)=2, a(9)=3, a(10)=3, a(11)=3
 * Range:     u8 n <= 140, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small table of initial values (needed for n < 12).  */
    static const seq_t init[12] = {
        1, 0, 1, 1, 1, 1,
        2, 2, 2, 3, 3, 3
    };

    if (n < 12U)
        return init[(size_t)n];

    /* 12‑slot sliding window that holds the last 12 values.
        The elements are updated iteratively; no dynamic memory
        allocation is required.  */
    seq_t wnd[12];
    for (size_t i = 0; i < 12; ++i)
        wnd[i] = init[i];

    for (idx_t i = 12; i <= n; ++i) {
        /* Compute a(i) from the recurrence.
           Indices inside wnd correspond to a(i-12) ... a(i-1).  */
        seq_t cur = wnd[10] + wnd[9] - wnd[7] + wnd[5]
                  - wnd[3] - wnd[2] + wnd[0];

        /* Shift window left by one: discard wnd[0], shift the rest.  */
        for (size_t k = 0; k < 11; ++k)
            wnd[k] = wnd[k + 1];
        wnd[11] = cur;   /* wnd[11] now holds a(i) */
    }

    return wnd[11];
}
