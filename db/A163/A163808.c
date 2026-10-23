/*
 * A163808  Write n in binary. Place a 0 right of every 1 without a 0 right of
 *          it. a(n) = the decimal value of the result.
 * https://oeis.org/A163808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = decimal value of the binary string obtained by writing n
 *            in binary and inserting a '0' immediately after each '1' that is
 *            not followed by a '0'. Example: n = 13 (1101₂) → 101010₂ = 42₁₀
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the (unlikely) zero case */
    if (n == 0)
        return (seq_t)0;

    /* Determine the number of bits in n */
    int bitlen = 0;
    idx_t tmp = n;
    while (tmp) {
        ++bitlen;
        tmp >>= 1;
    }

    /* Build the result bit by bit */
    seq_t result = 0;
    for (int i = bitlen - 1; i >= 0; --i) {
        int bit = (n >> i) & 1ULL;            /* current bit of n */
        result = (result << 1) | (seq_t)bit;  /* append this bit */

        /* If this bit is 1 and the next lower bit is not 0,
           then append an extra 0. */
        if (bit == 1) {
            if (i == 0 || ((n >> (i - 1)) & 1ULL) == 1) {
                result = result << 1;
            }
        }
    }
    return result;
}
