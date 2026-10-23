/*
 * A028741  Nonsquares mod 28.
 * https://oeis.org/A028741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 28*floor((n-1)/20) + r[(n-1)%20] where r is the set of
 *            residues modulo 28 that are NOT quadratic residues
 *            (2,3,5,6,7,10,11,12,13,14,15, 17,18,19,20,22,23,24,26,27)
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1 */

    /* Build the residue list on first call */
    static seq_t residues[20];
    static bool initialized = false;

    if (!initialized) {
        bool is_res[28] = { false };
        for (idx_t x = 0; x < 28; ++x) {
            idx_t sq = (x * x) % 28;   /* x < 28, product fits in idx_t */
            is_res[sq] = true;
        }
        idx_t count = 0;
        for (idx_t r = 0; r < 28; ++r) {
            if (!is_res[r]) {
                residues[count++] = (seq_t)r;
                if (count == 20) break;
            }
        }
        initialized = true;
    }

    idx_t  idx   = n - 1;
    idx_t  block = idx / 20;
    idx_t  pos   = idx % 20;

    return (seq_t)28 * (seq_t)block + residues[pos];
}
