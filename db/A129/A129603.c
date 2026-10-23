/*
 * A129603  Replace in the binary expansion of n each run of k 0's (or 1's)
 *          with 2k-1 0's (or 1's).
 * https://oeis.org/A129603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by replacing each run of equal bits in the
 *            binary * representation of n by a run of length 2k-1 of the same
 *            bit. * Example: n = 9 (1001₂) → runs: 1,00,1 → lengths 1,2,1 →
 *            new lengths 1,3,1 * → new binary 10001₂ = 17
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Append a run of `bit` repeated `len` times to `*res`. */
static void append_run(seq_t *res, int bit, unsigned len) {
    for (unsigned i = 0; i < len; ++i) {
        *res <<= 1;
        if (bit) {
            *res |= (seq_t)1;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }

    /* Find position of the most significant set bit of n. */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp >>= 1) {
        ++msb;
    }

    seq_t res = (seq_t)0;
    int prev_bit = 1;          /* high bit of n is always 1 */
    int run_len = 1;           /* current run length (in original bits) */

    /* Scan bits from the next higher position down to 0. */
    for (int pos = (int)msb - 1; pos >= 0; --pos) {
        int bit = (n >> pos) & 1;
        if (bit == prev_bit) {
            ++run_len;
        } else {
            /* Finalise the previous run. */
            unsigned new_len = (unsigned)(2 * run_len - 1);
            append_run(&res, prev_bit, new_len);
            prev_bit = bit;
            run_len = 1;
        }
    }

    /* Append the final run. */
    unsigned new_len = (unsigned)(2 * run_len - 1);
    append_run(&res, prev_bit, new_len);

    return res;
}
