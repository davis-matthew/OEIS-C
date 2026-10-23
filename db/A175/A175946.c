/*
 * A175946  List the run lengths of n's binary runs of zeros, then interpret
 *          this list as lengths of runs of alternating ones and zeros in
 *          binary.
 * https://oeis.org/A175946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = interpret run-lengths of zero runs in n's binary as
 *            alternating runs of 1s and 0s, starting with 1; convert the
 *            resulting binary number to decimal
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n == 0 will not occur for the OEIS offset, but handle safely */
    if (n == 0) return 0;

    seq_t out = 0;              /* result binary value */
    seq_t bit = 1;              /* current run: 1 for ones, 0 for zeros */

    /* Find the position of the most significant 1 bit (0‑based). */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        tmp >>= 1;
        ++msb;
    }

    /* Scan bits from msb‑1 down to 0 to collect zero runs. */
    bool in_zero = false;
    uint64_t zero_len = 0;

    for (int i = (int)msb - 1; i >= 0; --i) {
        bool b = ((n >> i) & 1ULL) != 0;   /* current bit of n */
        if (!b) {                          /* zero bit */
            if (in_zero)
                ++zero_len;
            else {
                in_zero = true;
                zero_len = 1;
            }
        } else {                           /* one bit */
            if (in_zero) {
                /* Emit the run of the current bit. */
                for (uint64_t k = 0; k < zero_len; ++k)
                    out = (out << 1) | bit;
                bit ^= 1;                  /* next run flips bit value */
            }
            in_zero = false;
        }
    }

    /* Process a trailing zero run, if present. */
    if (in_zero) {
        for (uint64_t k = 0; k < zero_len; ++k)
            out = (out << 1) | bit;
    }

    return out;
}
