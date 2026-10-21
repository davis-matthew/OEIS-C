/*
 * A221468  The Collatz (3x+1) iteration in A220145 converted to decimal.
 * https://oeis.org/A221468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = decimal representation of the binary string obtained by *
 *            listing the parity (1 for odd, 0 for even) of each term in the
 *            Collatz * sequence for n (including the final 1), then reversing
 *            the string
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 26, u64 n <= 26, u128 n <= 312,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is at least 1 by the sequence definition. */
    uint64_t x = (uint64_t)n;

    /* Buffer for parity bits.  The Collatz trajectory of n ≤ 10000
     * has fewer than 5 000 terms, which is far more than enough. */
    static unsigned char bits[5000];
    int len = 0;

    /* Generate parity bits of the Collatz sequence (including 1). */
    while (1)
    {
        bits[len++] = (x & 1ULL) ? 1 : 0;
        if (x == 1ULL)
            break;
        if ((x & 1ULL) == 0ULL)
            x >>= 1ULL;
        else
            x = 3ULL * x + 1ULL;
    }

    /* Build the decimal value by processing the bits in reverse order. */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i)
        res = (res << 1) | bits[i];

    return res;
}
