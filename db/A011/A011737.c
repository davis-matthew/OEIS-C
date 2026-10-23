/*
 * A011737  A binary m-sequence: expansion of reciprocal of x^24 + x^4 + x^3 +
 *          x + 1 (mod 2, shifted by 23 initial 0's).
 * https://oeis.org/A011737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for 0 ≤ n < 23 a(23) = 1 a(n) = a(n-24) XOR a(n-4) XOR
 *            a(n-3) XOR a(n-1) for n ≥ 24
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is the expansion of 1/(x^24+x^4+x^3+x+1) over GF(2) shifted
    by 23 leading zeros.  Only bits 0/1 are produced, so the algorithm
    uses a small sliding window of the last 24 computed bits. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 23)
        return 0;

    /* buf[i] holds a(i-24) … a(i-1) when the loop index is i */
    seq_t buf[24];
    for (int k = 0; k < 24; ++k)
        buf[k] = 0;
    buf[23] = 1;   /* a(23) */

    if (n == 23)
        return 1;

    /* Compute terms a(24) … a(n) sequentially. */
    for (idx_t i = 24; i <= n; ++i)
    {
        seq_t new_bit = buf[0] ^ buf[20] ^ buf[21] ^ buf[23];

        /* Shift left: drop a(i-24) and make room for new_bit */
        for (int j = 0; j < 23; ++j)
            buf[j] = buf[j + 1];
        buf[23] = new_bit;
    }
    return buf[23];
}
