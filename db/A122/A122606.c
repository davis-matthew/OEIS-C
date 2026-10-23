/*
 * A122606  n^(n+1) mod 7.
 * https://oeis.org/A122606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n+1) mod 7
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t base = (seq_t)(n % 7);
    if (base == 0)
        return 0;
    seq_t exp = (seq_t)((n + 1) % 6);
    seq_t result = 1;
    for (seq_t i = 0; i < exp; ++i)
        result = (result * base) % 7;
    return result;
}
