/*
 * A179821  In binary representation of n: replace all blocks of k contiguous
 *          ones with binary representation of k.
 * https://oeis.org/A179821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = transform(n) – replace each maximal block of k
 *            consecutive 1‑bits in the binary representation of n by the
 *            binary representation of k itself. For example, n = 45 (101101₂)
 *            → 101001₂ (decimal 41)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* floor_log2_u64 (n > 0).  Efficient for small values used below. */
static inline idx_t floor_log2_u64(idx_t n)
{
    idx_t r = 0;
    while ((n >>= 1) != 0) ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t  result = 0;          /* accumulated transformed bits    */
    idx_t  run    = 0;           /* current run of consecutive 1s  */

    /* Most significant bit index of n */
    idx_t  msb = floor_log2_u64(n);

    /* Process bits from MSB to LSB */
    for (idx_t i = msb;; --i) {
        bool bit = ((n >> i) & 1) != 0;
        if (bit) {
            ++run;                          /* extend current run */
        } else {
            if (run > 0) {                   /* finish run of 1s */
                idx_t lenbits = floor_log2_u64(run) + 1;
                result = (result << lenbits) | (seq_t)run;
                run = 0;
            }
            result <<= 1;                     /* append a single 0 bit */
        }

        if (i == 0) break;                   /* last bit processed */
    }

    /* If n ends with a run of 1s, append its representation */
    if (run > 0) {
        idx_t lenbits = floor_log2_u64(run) + 1;
        result = (result << lenbits) | (seq_t)run;
    }

    return result;
}
