/*
 * A078750  Least m not less than 2*n such that m + n = m OR n (logical 'or',
 *          bitwise).
 * https://oeis.org/A078750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the least m >= 2*n such that (m & n) == 0
 * (i.e. m + n = m OR n, equivalently addition without carries).
 * The code searches upwards from 2*n until a conflict-free m is found.
 */
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;          /* convert index to seq_t type */
    seq_t m  = nn << 1;           /* initial candidate 2*n */

    while ((m & nn) != 0) {
        /* find least significant bit where both m and n have a 1 */
        seq_t conflict = m & nn;
        idx_t i = 0;
        seq_t t = conflict;
        while ((t & 1) == 0) {
            t >>= 1;
            ++i;
        }
        /* set a higher bit that is not conflicted and bump to the next number */
        m = (m | ((seq_t)1 << (i + 1))) + 1;
    }
    return m;
}
