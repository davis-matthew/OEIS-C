/*
 * A175930  Concatenation of run lengths in binary expansion of n, written in
 *          base 2, then converted to base 10.
 * https://oeis.org/A175930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, look, nonn
 * Formula:   a(n) = concatenation of the run lengths of the binary *
 *            representation of n (each run length written in binary), then *
 *            interpreted as a base‑10 integer
 * Range:     u8 n <= 291, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return the number of bits needed to represent x (x > 0). */
static inline idx_t bitlen_idx_t(idx_t x)
{
    idx_t len = 0;
    while (x) {
        x >>= 1;
        ++len;
    }
    return len;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence */
    seq_t result = 0;

    /* Find position of the most significant set bit (0‑based). */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp >> 1) {
        tmp >>= 1;
        ++msb;
    }

    /* Initialize the first run. */
    int current_bit = (int)((n >> msb) & 1ULL);
    idx_t run_len = 1;

    /* Iterate over the remaining bits from msb‑1 down to 0. */
    for (idx_t i = msb; i > 0; --i) {
        int bit = (int)((n >> (i - 1)) & 1ULL);
        if (bit == current_bit) {
            /* Still the same run: increase its length. */
            ++run_len;
        } else {
            /* Finish current run: append its binary representation to result. */
            idx_t bits_needed = bitlen_idx_t(run_len);
            result <<= bits_needed;
            result |= (seq_t)run_len;

            /* Start a new run. */
            current_bit = bit;
            run_len = 1;
        }
    }

    /* Append the final run. */
    idx_t bits_needed = bitlen_idx_t(run_len);
    result <<= bits_needed;
    result |= (seq_t)run_len;

    return result;
}
