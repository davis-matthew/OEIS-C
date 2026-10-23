/*
 * A175945  Start with the binary expansion of n; list the lengths of the runs
 *          of ones and interpret this list as lengths of runs of alternating
 *          ones and zeros in binary.
 * https://oeis.org/A175945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = interpret the list of run–lengths of 1s in the binary
 *            expansion of n as run–lengths of alternating runs of 1s and 0s,
 *            starting with 1, forming a binary number
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for n=0, handled */

    /* Determine most significant bit (position of highest 1) */
    unsigned msb = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        ++msb;
    }

    /* Collect lengths of runs of 1s in the binary representation of n */
    unsigned char run_len[64];
    unsigned run_cnt = 0;
    bool in_run = false;
    unsigned current_len = 0;

    for (int i = (int)msb; i >= 0; --i) {
        bool bit = ((n >> i) & 1ULL) != 0;
        if (bit) {
            if (!in_run) {
                in_run = true;
                current_len = 1;
            } else {
                ++current_len;
            }
        } else {
            if (in_run) {
                run_len[run_cnt++] = (unsigned char)current_len;
                in_run = false;
            }
        }
    }
    if (in_run) {
        run_len[run_cnt++] = (unsigned char)current_len;
    }

    /* Build new binary number from these run‑lengths */
    const unsigned width = sizeof(seq_t) * 8;
    seq_t res = 0;
    const seq_t mask_all = ~((seq_t)0);

    for (unsigned i = 0; i < run_cnt; ++i) {
        unsigned L = run_len[i];

        /* Shift left by L bits, handling L >= width explicitly */
        if (L >= width) {
            res = 0;
        } else {
            res <<= L;
        }

        /* If this run is of 1s (odd index 0‑based), set the lowest L bits to 1 */
        if ((i & 1) == 0) { /* run of 1s */
            if (L >= width) {
                res |= mask_all;
            } else {
                seq_t mask = ((seq_t)1 << L) - 1;
                res |= mask;
            }
        }
        /* Runs of 0s require no additional action */
    }

    return res;
}
