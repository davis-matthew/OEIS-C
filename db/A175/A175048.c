/*
 * A175048  Write n in binary, then increase each run of 1's by one 1. a(n) is
 *          the decimal equivalent of the result.
 * https://oeis.org/A175048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is obtained by taking the binary representation of n and
 *            inserting an additional '1' immediately after every run of 1's.
 *            a(n) is the decimal value of the resulting binary string
 * Range:     u8 n <= 36, u16 n <= 1364, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Treat n as a non‑zero positive integer. */
    seq_t result = 0;
    bool in_run   = false;

    /* Find the position of the most significant set bit. */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp >>= 1) {
        ++msb;
    }

    /* Process bits from most significant to least. */
    for (idx_t i = msb;; --i) {
        bool bit = (n >> i) & 1;

        if (bit) {
            /* Append the original 1 bit. */
            result = (result << 1) | 1;
            in_run = true;
        } else {
            if (in_run) {
                /* End of a run – append the extra 1. */
                result = (result << 1) | 1;
                in_run = false;
            }
            /* Append the original 0 bit. */
            result = result << 1;
        }

        if (i == 0) break;
    }

    /* If n ends with a run of ones, add the final inserted 1. */
    if (in_run) {
        result = (result << 1) | 1;
    }

    return result;
}
