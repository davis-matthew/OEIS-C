/*
 * A235897  Number of (n+1) X (3+1) 0..2 arrays with the minimum plus the
 *          maximum equal to the lower median plus the upper median in every 2
 *          X 2 subblock.
 * https://oeis.org/A235897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence of order 7: For n >= 8 a(n) =
 *            21*a(n-1) - 124*a(n-2) - 72*a(n-3) + 2128*a(n-4) - 432*a(n-5) -
 *            12608*a(n-6) - 10752*a(n-7) Initial values: a(1) = 473, a(2) =
 *            3265, a(3) = 21697, a(4) = 148945, a(5) = 1030145, a(6) =
 *            7234417, a(7) = 51386977
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial terms 1..7 */
    static const seq_t init[8] = {
        0,      /* unused 0 index */
        473ULL, 3265ULL, 21697ULL, 148945ULL,
        1030145ULL, 7234417ULL, 51386977ULL
    };

    if(n <= 7) return init[n];

    /* Store last 7 values: v[0] = a(i-7), v[6] = a(i-1) for current i */
    seq_t v[7];
    v[0] = init[1];
    v[1] = init[2];
    v[2] = init[3];
    v[3] = init[4];
    v[4] = init[5];
    v[5] = init[6];
    v[6] = init[7];

    for(idx_t i = 8; i <= n; ++i) {
        /* compute a(i) */
        seq_t next = 21 * v[6]
                     - 124 * v[5]
                     - 72  * v[4]
                     + 2128 * v[3]
                     - 432  * v[2]
                     - 12608 * v[1]
                     - 10752 * v[0];

        /* shift values for next iteration */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = v[6];
        v[6] = next;
    }

    return v[6];
}
