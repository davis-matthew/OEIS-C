/*
 * A211687  Number of -4..4 arrays x(i) of n+1 elements i=1..n+1 with
 *          x(i)+x(j), x(i+1)+x(j+1), -(x(i)+x(j+1)), and -(x(i+1)+x(j))
 *          having three distinct values for every i<=n and j<=n.
 * https://oeis.org/A211687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence (order 12): a(1)=68, a(2)=156, a(3)=318,
 *            a(4)=604, a(5)=1144, a(6)=2108, a(7)=3924, a(8)=7236,
 *            a(9)=13486, a(10)=25108, a(11)=47168, a(12)=88856. For n>12,
 *            a(n)=4*a(n-1)+6*a(n-2)-41*a(n-3)+6*a(n-4)+154*a(n-5)-109*a(n-6)
 *            -256*a(n-7)+262*a(n-8)+175*a(n-9)-230*a(n-10)-30*a(n-11)...
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for n = 1..12 */
    static const seq_t base_vals[13] = {
        0ULL, /* unused 0th element */
        68ULL,     /* a(1) */
        156ULL,    /* a(2) */
        318ULL,    /* a(3) */
        604ULL,    /* a(4) */
        1144ULL,   /* a(5) */
        2108ULL,   /* a(6) */
        3924ULL,   /* a(7) */
        7236ULL,   /* a(8) */
        13486ULL,  /* a(9) */
        25108ULL,  /* a(10) */
        47168ULL,  /* a(11) */
        88856ULL   /* a(12) */
    };

    if (n <= 12) {
        return base_vals[(size_t)n];
    }

    /* Use a sliding window of the last 12 terms to avoid allocating
       O(n) memory.  last[0] holds a(k-1), last[1] holds a(k-2), …,
       last[11] holds a(k-12).  After the loop, last[0] contains a(n). */
    seq_t last[12];
    for (int i = 0; i < 12; ++i) {
        last[i] = base_vals[12 - i];
    }

    for (idx_t k = 13; k <= n; ++k) {
        seq_t cur = 4 * last[0] + 6 * last[1] - 41 * last[2] + 6 * last[3]
                  + 154 * last[4] - 109 * last[5] - 256 * last[6]
                  + 262 * last[7] + 175 * last[8] - 230 * last[9]
                  - 30 * last[10] + 60 * last[11];

        /* Shift the window: drop the oldest term and insert cur at front */
        for (int i = 11; i > 0; --i) {
            last[i] = last[i - 1];
        }
        last[0] = cur;
    }

    return last[0];
}
