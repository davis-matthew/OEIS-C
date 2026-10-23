/*
 * A124765  Number of monotonically decreasing runs for compositions in
 *          standard order.
 * https://oeis.org/A124765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = 1 + (# of indices i>0 with part_i > part_{i-1}) for the
 *            composition of n obtained from the standard order (A066099). For
 *            n = 0, a(0) = 0
 * Range:     u8 n <= 8711, u16 n <= 8711, u32 n <= 8711, u64 n <= 8711,
 *            u128 n <= 8711, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0U;

    /* Generate the composition of index n in standard order. */
    uint64_t k = n;              /* k is the composition index */
    uint64_t pos = 1;            /* position counter, first bit is at 1 */
    uint64_t prev = 0;           /* previous 1-bit position */
    int len = 0;                 /* number of parts in the composition */
    seq_t tmp[64];               /* temporary storage of differences */

    while (k > 0) {
        if (k & 1ULL) {
            tmp[len++] = (seq_t)(pos - prev); /* difference between positions */
            prev = pos;
        }
        pos++;
        k >>= 1ULL;
    }

    /* Reverse the differences to obtain the composition parts. */
    seq_t parts[64];
    for (int i = 0; i < len; ++i)
        parts[i] = tmp[len - 1 - i];

    /* Count monotonically decreasing runs:
       start with one run, and start a new run whenever a part increases
       over the previous part. */
    seq_t runs = 1U;
    for (int i = 1; i < len; ++i)
        runs += (seq_t)(parts[i] > parts[i - 1]);

    return runs;
}
