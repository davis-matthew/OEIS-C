/*
 * A187171  Number of 10-step self-avoiding walks on an n X n X n cube summed
 *          over all starting positions.
 * https://oeis.org/A187171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1853886*n^3 - 12825630*n^2 + 27515184*n - 18252624 for
 *            n>8 a(1)=0, a(2)=0, a(3)=414912, a(4)=6654048, a(5)=30938640,
 *            a(6)=85654320, a(7)=181790352, a(8)=330218544
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 15, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 8) {
        switch ((idx_t)n) {
            case 1: return (seq_t)0;
            case 2: return (seq_t)0;
            case 3: return (seq_t)414912;
            case 4: return (seq_t)6654048;
            case 5: return (seq_t)30938640;
            case 6: return (seq_t)85654320;
            case 7: return (seq_t)181790352;
            case 8: return (seq_t)330218544;
            default: return (seq_t)0;
        }
    }
    uint64_t N = (uint64_t)n;  /* Convert index to 64‑bit for safe arithmetic */
    uint64_t res = 1853886ull * N * N * N
                 - 12825630ull * N * N
                 + 27515184ull * N
                 - 18252624ull;
    return (seq_t)res;
}
