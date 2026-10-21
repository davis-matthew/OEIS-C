/*
 * A235012  Number of (n+1) X (3+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2) - 30*a(n-3) + 7*a(n-4) + 74*a(n-5) -
 *            46*a(n-6) - 60*a(n-7) + 48*a(n-8) for n > 8 Initial values:
 *            a(1)=264,a(2)=488,a(3)=936,a(4)=1912,a(5)=4008,
 *            a(6)=8760,a(7)=19560,a(8)=44952
 * Range:     u8 none, u16 n <= 8, u32 n <= 20, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* First eight terms – they are needed to start the recurrence. */
    static const seq_t init[8] = {
        264ULL, 488ULL, 936ULL,
        1912ULL, 4008ULL, 8760ULL,
        19560ULL, 44952ULL
    };

    if (n <= 8) return init[n-1];

    /* Sliding window of the last eight terms.
       v[0] = a(k-8), ..., v[7] = a(k-1) when k is the next index to compute. */
    seq_t v[8];
    for (idx_t i = 0; i < 8; ++i) v[i] = init[i];

    for (idx_t k = 9; k <= n; ++k) {
        /* Compute a(k) from the eight previous terms in v. */
        seq_t next = v[7]*4   + v[6]*4   - v[5]*30
                     + v[4]*7   + v[3]*74  - v[2]*46
                     - v[1]*60  + v[0]*48;  /* Note: indices reversed to match notation */

        /* Shift the window one step forward. */
        for (idx_t i = 0; i < 7; ++i) v[i] = v[i+1];
        v[7] = next;
    }

    return v[7];
}
