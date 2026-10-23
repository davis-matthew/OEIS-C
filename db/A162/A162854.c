/*
 * A162854  Take the binary representation of n. Reduce by half the number of
 *          digits in every run (completely of either 0's or 1's) of an even
 *          number of digits. Double the number of digits in every run of an
 *          odd number of digits in the binary representation of n (where the
 *          added digits have the same value th...
 * https://oeis.org/A162854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   add_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A162854
 *
 *  Given n ≥ 1, write n in binary (no leading zeros).  For each maximal run
 *  of equal bits (starting from the most‑significant bit) do:
 *
 *        – if the run length is even, replace it by a run of the same bit
 *          with half as many digits.
 *        – if the run length is odd, replace it by a run of the same bit
 *          with double the number of digits.
 *
 *  The modified binary string is interpreted as a binary number and
 *  returned as a(n).
 *
 *  The algorithm runs in O(overall run length) = O(log n) time.
 */

#include <stdint.h>          /* for uint64_t */
#include "../utils/add_run.h"

/* Returns the 1‑based index of the least significant set bit of x
 *  (i.e. bit length of x).  For x = 0 the result is 0. */
static inline int bitlen_uint64(uint64_t x)
{
    int len = 0;
    while (x) { x >>= 1; ++len; }
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* not used – the sequence starts at index 1 */
        return 0;

    seq_t result = 0;

    int bits   = bitlen_uint64((uint64_t)n);   /* number of binary digits of n */
    int msb    = bits - 1;                     /* index of most significant bit */
    int cur    = (int)((n >> msb) & 1ULL);     /* value of first run */
    int runlen = 1;                            /* current run length */

    for (int i = msb - 1; i >= 0; --i) {
        int bit = (int)((n >> i) & 1ULL);
        if (bit == cur)
            ++runlen;
        else {
            add_run(&result, runlen, cur);
            cur    = bit;
            runlen = 1;
        }
    }
    add_run(&result, runlen, cur);   /* process final run */

    return result;
}
