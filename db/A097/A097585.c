/*
 * A097585  Binary representation of n if n is prime, otherwise the ternary
 *          representation of n.
 * https://oeis.org/A097585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS definition of A097585
 *   a(n) = binary representation of n if n is prime;
 *   otherwise a(n) = ternary representation of n.
 * The result is returned as a decimal integer (e.g. binary 12 -> 110,
 * ternary 5 -> 12).
 *
 * It uses the helper function is_prime(idx_t) from the utilities library.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* choose base: 2 for prime, 3 otherwise */
    int base = is_prime(n) ? 2 : 3;

    /* convert n to the chosen base, store digits LSB-first */
    idx_t tmp = n;
    int digs[64];
    int dlen = 0;
    do
    {
        digs[dlen++] = (int)(tmp % base);
        tmp /= base;
    } while (tmp > 0);

    /* build the decimal integer from most significant digit to least */
    seq_t result = 0;
    for (int i = dlen - 1; i >= 0; --i)
    {
        result = result * 10 + (seq_t)digs[i];
    }

    return result;
}
