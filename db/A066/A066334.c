/*
 * A066334  Binary string which equals n when 1's, 2's, 4's and 8's bits have
 *          weights 1, 2, 4, 2 respectively, while the other bits have their
 *          usual weights. -1 if no such string exists.
 * https://oeis.org/A066334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 31, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   msb_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: binary string whose special weighted sum equals n. 
 * Bits 0‑3 have weights 1,2,4,2 respectively; bits i≥4 have weight 2ⁱ. 
 * For a given n the smallest (in numeric value) binary string achieving the
 * weighted sum is returned, written as a decimal integer composed of only 
 * digits 0 and 1.  If no such string exists the function returns –1. 
 * The minimal pattern for the low four bits is:
 *   low_sum 0→0, 1→1, 2→2, 3→3, 4→4, 5→5, 6→6, 7→7, 8→1110 (14), 9→1111 (15). 
 */

#include <stdint.h>
#include "../utils/msb_uint64.h"

seq_t a(idx_t n) {
    /* Pre‑computed minimal low‑bit patterns (as integer values). */
    static const seq_t low_pat[10] = {0,1,2,3,4,5,6,7,14,15};

    /* n = 16·high + low, low ∈ [0,15] */
    uint64_t high = n >> 4;     /* n / 16 */
    uint64_t low  = n & 15;     /* n % 16 */

    if (low > 9)                    /* unreachable weighted sums */
        return (seq_t)-1;

    seq_t pattern = low_pat[low];   /* low‑bit pattern (0‑15) */

    /* Full binary integer whose bits represent the desired string. */
    seq_t full = ((seq_t)high << 4) | pattern;

    if (full == 0)          /* a(0) = 0 */
        return 0;

    /* Find most significant bit of the full integer. */
    int msb;
    if (high != 0) {
        int msb_high = msb_uint64(high);
        msb = msb_high + 4;    /* shift of low bits adds 4 positions */
    } else {
        msb = msb_uint64((uint64_t)pattern);
        if (msb < 0)          /* should not happen, but safety */
            return 0;
    }

    /* Convert the binary representation into a decimal integer
     * consisting only of digits 0 and 1. */
    seq_t result = 0;
    for (int pos = msb; pos >= 0; --pos) {
        int bit = (int)((full >> pos) & 1);
        result = result * 10 + bit;
    }

    return result;
}
