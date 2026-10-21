/*
 * A200058  Number of -n..n arrays x(0..3) of 4 elements with zero sum and
 *          elements alternately strictly increasing and strictly decreasing.
 * https://oeis.org/A200058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 3*a(n-1) -3*a(n-2) +2*a(n-3) -3*a(n-4) +3*a(n-5) -a(n-6), n≥7
   Initial values: a(1)=4, a(2)=26, a(3)=78, a(4)=172, a(5)=324, a(6)=546. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static idx_t cap = 0;
    static idx_t max_computed = 0;
    static const seq_t init[7] = {0, 4, 26, 78, 172, 324, 546};

    if (n <= 6)
        return init[n];

    if (!buf) {
        cap = 8; /* enough for indices 0..7 */
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        for (idx_t i = 0; i < 7; ++i)
            buf[i] = init[i];
        max_computed = 6;
    }

    if (n + 1 > cap) {
        idx_t new_cap = cap;
        while (new_cap <= n)
            new_cap <<= 1;
        buf = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val = 3 * buf[i-1] - 3 * buf[i-2] + 2 * buf[i-3] - 3 * buf[i-4] + 3 * buf[i-5] - buf[i-6];
        buf[i] = val;
    }
    max_computed = n;
    return buf[n];
}
