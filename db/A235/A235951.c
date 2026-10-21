/*
 * A235951  Number of (n+1) X (5+1) 0..2 arrays colored with the upper median
 *          value of each 2 X 2 subblock.
 * https://oeis.org/A235951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-4) for n > 13, with the initial values a(1) … a(13)
 *            specified in the OEIS entry
 * Range:     u8 none, u16 n <= 12, u32 n <= 44, u64 n <= 108, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 1‑based initial values for 1 ≤ n ≤ 13 */
    static const unsigned long long init[14] = {
        0ULL,         /* placeholder for index 0 */
        7353ULL,      /* a(1) */
        5034ULL,      /* a(2) */
        2478ULL,      /* a(3) */
        3496ULL,      /* a(4) */
        5358ULL,      /* a(5) */
        6728ULL,      /* a(6) */
        9260ULL,      /* a(7) */
        14016ULL,     /* a(8) */
        20570ULL,     /* a(9) */
        26912ULL,     /* a(10) */
        36512ULL,     /* a(11) */
        56064ULL,     /* a(12) */
        82568ULL      /* a(13) */
    };

    if (n <= 13) {
        return (seq_t) init[(size_t)n];
    }

    /* For n > 13, repeatedly use the recurrence a(n)=4*a(n-4). */
    idx_t t = (n - 10) / 4;          /* how many times to apply the recurrence */
    idx_t m = n - 4 * t;             /* resulting index in 1..13 */

    seq_t result = (seq_t) init[(size_t)m];

    /* Multiply by 4^t; do the multiplication iteratively to avoid
       shifting by more bits than the type allows. */
    for (idx_t i = 0; i < t; ++i) {
        result *= 4;
    }

    return result;
}
