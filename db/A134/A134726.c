/*
 * A134726  Concatenation of next n cubes of positive integers.
 * https://oeis.org/A134726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concatenation of the next n cubes of positive integers
 *            where the sequence of integers runs without gaps: start(n) = 1 +
 *            (n-1)*n/2. For each k = 0 … n-1 compute cur = start(n) + k cube
 *            = cur^3 digits = number of decimal digits of cube result =
 *            result * 10^digits + cube The utilities pow10(i...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t start = (seq_t)n * ((seq_t)n - 1) / 2 + 1;
    seq_t result = 0;

    for (idx_t i = 0; i < n; ++i)
    {
        seq_t cur   = start + (seq_t)i;
        seq_t cube  = cur * cur * cur;
        int    digs = digit_len(cube);
        result = result * pow10((idx_t)digs) + cube;
    }
    return result;
}
