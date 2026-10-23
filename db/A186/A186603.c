/*
 * A186603  Number of (n+4)X7 binary arrays with every 5X5 subblock commuting
 *          with each horizontal and vertical neighbor 5X5 subblock.
 * https://oeis.org/A186603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 64, u64 n <= 154, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A186603: Number of (n+4)×7 binary arrays with every 5×5 subblock commuting
   with each horizontal and vertical neighbor 5×5 subblock.
   Empirical recurrence (valid for n>19):
   a(n) = a(n-1)+a(n-3)-a(n-4)+a(n-5)-a(n-6)-a(n-8)+a(n-9)+a(n-10)-a(n-13)
   Initial values: a(1)=11884, a(2)=916, … , a(19)=327317
*/

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /*----- initial values up to a(19) ---------------------------------------*/
    static const seq_t init[20] = {
        0,          /* dummy 0 to keep indices 1‑based */
        11884,      /* a(1) */
        916,        /* a(2) */
        1566,       /* a(3) */
        5813,       /* a(4) */
        19840,      /* a(5) */
        44120,      /* a(6) */
        61336,      /* a(7) */
        66332,      /* a(8) */
        67552,      /* a(9) */
        68580,      /* a(10) */
        69616,      /* a(11) */
        70680,      /* a(12) */
        72140,      /* a(13) */
        75248,      /* a(14) */
        88073,      /* a(15) */
        126738,     /* a(16) */
        189047,     /* a(17) */
        256748,     /* a(18) */
        327317      /* a(19) */
    };

    /* If the requested index is outside the defined range, return 0. */
    if (n < 1) return 0;

    /* Return the pre‑computed terms directly for n ≤ 19. */
    if (n <= 19) return init[n];

    /* Circular buffer holding the last 14 computed terms.
       buffer[(k-1)%14] is guaranteed to hold a(k) for all already computed k ≥ 6. */
    static seq_t buffer[14];
    static idx_t computed = 0;   /* maximal n for which buffer contains correct data */
    static bool initialized = false;

    if (!initialized) {
        /* initialise buffer with a(6) … a(19) */
        for (idx_t k = 6; k <= 19; ++k)
            buffer[(k - 1) % 14] = init[k];
        computed = 19;
        initialized = true;
    }

    /* If the requested term has already been computed, return it. */
    if (n <= computed)
        return buffer[(n - 1) % 14];

    /* Compute subsequent terms using the empirical recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val =
            buffer[(i - 2) % 14] +   /* a(i-1) */
            buffer[(i - 4) % 14] -   /* a(i-3) */
            buffer[(i - 5) % 14] +   /* a(i-4) */
            buffer[(i - 6) % 14] -   /* a(i-5) */
            buffer[(i - 7) % 14] -   /* a(i-6) */
            buffer[(i - 9) % 14] +   /* a(i-8) */
            buffer[(i -10) % 14] +   /* a(i-9) */
            buffer[(i -11) % 14] -   /* a(i-10) */
            buffer[(i -14) % 14];    /* a(i-13) */

        buffer[(i - 1) % 14] = val;
    }
    computed = n;

    return buffer[(n - 1) % 14];
}
