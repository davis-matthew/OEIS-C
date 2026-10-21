/*
 * A084188  a(0)=1, a(n+1) = 2*a(n) + b(n+2), where b(n)=A004539(n) is the
 *          n-th bit in the binary expansion of sqrt(2).
 * https://oeis.org/A084188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(2) * 2^n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic when seq_t has up to 8 bytes.  This handles
       all cases where the result really fits into a built‑in type.  For
       larger seq_t (e.g. arbitrary‑precision), perform the calculation
       directly with seq_t operations. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 r = 0;
        unsigned __int128 root = 0;
        for (idx_t i = 0; i <= n; ++i) {
            unsigned int pair = (i == 0) ? 2 : 0;
            r = (r << 2) | pair;
            root <<= 1;
            unsigned __int128 trial = (root << 1) | 1;
            if (r >= trial) {
                r -= trial;
                root |= 1;
            }
        }
        return (seq_t)root;
    }
    else {
        seq_t r = 0;
        seq_t root = 0;
        for (idx_t i = 0; i <= n; ++i) {
            seq_t pair = (i == 0) ? (seq_t)2 : (seq_t)0;
            r = (r << 2) | pair;
            root <<= 1;
            seq_t trial = (root << 1) | (seq_t)1;
            if (r >= trial) {
                r -= trial;
                root |= (seq_t)1;
            }
        }
        return root;
    }
}
