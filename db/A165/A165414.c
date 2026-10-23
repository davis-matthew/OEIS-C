/*
 * A165414  a(n) = number of run-lengths that each occur only once in the
 *          binary representation of n.
 * https://oeis.org/A165414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of run‑lengths that each occur only once in the
 *            binary representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* n>=1 in OEIS, safety guard */

    uint64_t x = (uint64_t)n;
    /* find index of most significant set bit */
    int msb = 0;
    uint64_t tmp = x;
    while (tmp >>= 1) msb++;

    /* store run lengths (max 64 runs for 64‑bit n) */
    int runLengths[65];
    int runs = 0;
    int curBit = (x >> msb) & 1U;
    int curRun = 1;

    for (int i = msb - 1; i >= 0; --i) {
        int bit = (x >> i) & 1U;
        if (bit == curBit) {
            ++curRun;
        } else {
            runLengths[runs++] = curRun;
            curBit = bit;
            curRun = 1;
        }
    }
    runLengths[runs++] = curRun;          /* final run */

    /* count frequencies of each run length (max length 64) */
    int freq[65] = {0};
    for (int i = 0; i < runs; ++i) {
        int len = runLengths[i];
        if (len >= 0 && len <= 64) freq[len]++;
    }

    /* count run lengths that appear exactly once */
    int distinct = 0;
    for (int len = 1; len <= 64; ++len) {
        if (freq[len] == 1) ++distinct;
    }

    return (seq_t)distinct;
}
