/*
 * A212400  Number of binary arrays of length n+11 with no more than 6 ones in
 *          any length 12 subsequence (=50% duty cycle).
 * https://oeis.org/A212400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary arrays of length n+11 that contain at
 *            most six 1‑entries in every 12 consecutive positions
 *            (equivalently, every length‑12 sliding window has ≤6 ones). The
 *            value is obtained by a linear‑time DP with a state equal to the
 *            last 11 bits
 * Range:     u8 none, u16 n <= 6, u32 n <= 24, u64 n <= 60, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Ensure that seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* length of the binary array */
    idx_t L = n + 11;          /* we need the counts for lengths 0 … L */

    /* Transition states: last 11 bits of the already built prefix.  */
    static seq_t prev[2048];
    static seq_t cur[2048];
    static unsigned char popc[2048];
    static int popc_init = 0;

    if (!popc_init) {
        for (int i = 0; i < 2048; ++i)
            popc[i] = (unsigned char)__builtin_popcount((unsigned)i);
        popc_init = 1;
    }

    /* initialise DP for length 0 */
    for (int i = 0; i < 2048; ++i) prev[i] = 0;
    prev[0] = (seq_t)1;  /* empty prefix */

    /* perform the DP until the desired length L is reached */
    for (idx_t step = 0; step < L; ++step) {
        /* zero the array that will hold the new counts */
        for (int i = 0; i < 2048; ++i) cur[i] = 0;

        /* transition from all previous states */
        for (int mask = 0; mask < 2048; ++mask) {
            seq_t val = prev[mask];
            if (!val) continue;

            /* adding a 0 bit */
            int new_mask0 = (mask << 1) & 0x7FF;
            if (step < 11 || popc[mask] + 0 <= 6)
                cur[new_mask0] = cur[new_mask0] + val;

            /* adding a 1 bit */
            int new_mask1 = ((mask << 1) | 1) & 0x7FF;
            if (step < 11 || popc[mask] + 1 <= 6)
                cur[new_mask1] = cur[new_mask1] + val;
        }

        /* move to the next step */
        for (int i = 0; i < 2048; ++i) prev[i] = cur[i];
    }

    /* after processing L bits, sum all states to obtain the total
       number of valid arrays of length L = n+11 */
    seq_t total = 0;
    for (int i = 0; i < 2048; ++i)
        total = total + prev[i];

    return total;
}
