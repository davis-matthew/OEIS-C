/*
 * A105500  Trajectory of 1 under the morphism 1->{1,2}, 2->{3,2}, 3->{3,4},
 *          4->{1,4}.
 * https://oeis.org/A105500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A246960(n) + 1 (trajectory of 1 under the morphism 1 →
 *            {1,2}, 2 → {3,2}, 3 → {3,4}, 4 → {1,4})
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* mapping tables for the first and second images */
    static const unsigned int first_map[5]  = {0, 1, 3, 3, 1};
    static const unsigned int second_map[5] = {0, 2, 2, 4, 4};

    if (n == 0)                 /* a(0) = 1 */
        return (seq_t)1;

    unsigned int cur = 1;        /* start from symbol 1 */

    /* find most significant set bit of n (n > 0) */
    idx_t msb = 63;
    while (msb > 0 && ((n >> msb) == 0))
        --msb;

    /* walk from that bit down to bit 0 */
    for (idx_t k = msb; ; --k) {
        if ((n >> k) & 1)       /* choose second image if bit is 1 */
            cur = second_map[cur];
        else
            cur = first_map[cur];   /* choose first image if bit is 0 */

        if (k == 0) break;          /* avoid underflow of unsigned k */
    }

    return (seq_t)cur;
}
