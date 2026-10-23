/*
 * A183963  Number of (n+1)X2 0..2 arrays with no 2X2 subblock sum differing
 *          from a horizontal or vertical neighbor subblock sum by more than
 *          one.
 * https://oeis.org/A183963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1)+6*a(n-3)+81*a(n-4)-468*a(n-5)-72*a(n-6)-432*a(n-8)+1728*a(n-9)
   Initial values for n = 1 … 9 are known (a(1)=81, a(2)=459, a(3)=2601, a(4)=15759, a(5)=95481,
   a(6)=586791, a(7)=3606201, a(8)=22235391, a(9)=137100681). */

#include <stddef.h>   /* for size_t */

static const seq_t INITIAL_VALUES[10] = {
    (seq_t)0,
    (seq_t)81,            /* a(1) */
    (seq_t)459,           /* a(2) */
    (seq_t)2601,          /* a(3) */
    (seq_t)15759,         /* a(4) */
    (seq_t)95481,         /* a(5) */
    (seq_t)586791,        /* a(6) */
    (seq_t)3606201,       /* a(7) */
    (seq_t)22235391,      /* a(8) */
    (seq_t)137100681      /* a(9) */
};

seq_t a(idx_t n) {
    if (n <= 9) {
        return INITIAL_VALUES[n];
    }

    /* A circular buffer holding the last nine computed terms.
       ring[i] holds a(current_index - 9 + i) for current_index
       (i from 0 to 8). */
    seq_t ring[9];
    /* Fill ring with a(1) … a(9) */
    for (idx_t k = 1; k <= 9; ++k) {
        ring[(k - 1) % 9] = INITIAL_VALUES[k];
    }

    seq_t term;
    for (idx_t i = 10; i <= n; ++i) {
        /* Helper to get a(i - k) from ring */
        #define GET(k) ring[ ( (i - (k) - 1 + 9) % 9 ) ]

        term = (seq_t)6   * GET(1) +
               (seq_t)6   * GET(3) +
               (seq_t)81  * GET(4) -
               (seq_t)468 * GET(5) -
               (seq_t)72  * GET(6) -
               (seq_t)432 * GET(8) +
               (seq_t)1728 * GET(9);

        ring[(i - 1) % 9] = term;
        #undef GET
    }

    return ring[(n - 1) % 9];
}
