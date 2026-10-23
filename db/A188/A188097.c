/*
 * A188097  Number of (n+3) X 4 binary arrays with every 4 X 4 subblock
 *          commuting with each horizontal and vertical neighbor 4 X 4
 *          subblock.
 * https://oeis.org/A188097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n > 15, a(n) = 2*a(n-1) - a(n-2) + a(n-4) - 2*a(n-5) +
 *            a(n-6) + a(n-8) - a(n-9) Initial values (for 1 ≤ n ≤ 15): a(1) =
 *            65536, a(2) = 708, a(3) = 1417, a(4) = 3594, a(5) = 8274, a(6) =
 *            13814, a(7) = 17850, a(8) = 20888, a(9) = 24117, a(10) = 26596,
 *            a(11) = 29585, a(12) = 35384, a(13) = 46623,...
 * Range:     u8 none, u16 none, u32 n <= 58, u64 n <= 148, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Memoization using static storage */
    static seq_t *seq = NULL;
    static size_t cap   = 0;   /* allocated capacity (max index that can be accessed) */
    static size_t last  = 0;   /* highest index computed so far */

    /* First call: allocate and initialise base terms */
    if (!seq) {
        cap = 16;                     /* capacity at least 16 (>15) */
        seq = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        if (!seq) { /* allocation failed – abort */ abort(); }

        seq[0] = 0;                    /* unused */
        seq[1] = (seq_t)65536;
        seq[2] = (seq_t)708;
        seq[3] = (seq_t)1417;
        seq[4] = (seq_t)3594;
        seq[5] = (seq_t)8274;
        seq[6] = (seq_t)13814;
        seq[7] = (seq_t)17850;
        seq[8] = (seq_t)20888;
        seq[9] = (seq_t)24117;
        seq[10] = (seq_t)26596;
        seq[11] = (seq_t)29585;
        seq[12] = (seq_t)35384;
        seq[13] = (seq_t)46623;
        seq[14] = (seq_t)63199;
        seq[15] = (seq_t)84577;
        last = 15;
    }

    if (n <= last) {
        return seq[(size_t)n];
    }

    /* Ensure capacity */
    if ((size_t)n > cap) {
        size_t new_cap = cap;
        while (new_cap < (size_t)n) {
            new_cap <<= 1;            /* double until large enough */
        }
        seq = (seq_t *)realloc(seq, (new_cap + 1) * sizeof(seq_t));
        if (!seq) { /* reallocation failed – abort */ abort(); }
        cap = new_cap;
    }

    /* Compute successive values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq[(size_t)i] =
            2 * seq[(size_t)(i - 1)]
            - seq[(size_t)(i - 2)]
            + seq[(size_t)(i - 4)]
            - 2 * seq[(size_t)(i - 5)]
            + seq[(size_t)(i - 6)]
            + seq[(size_t)(i - 8)]
            - seq[(size_t)(i - 9)];
    }
    last = (size_t)n;
    return seq[(size_t)n];
}
