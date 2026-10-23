/*
 * A106649  Replace each digit d (except the leading one) of n with 9-d.
 * https://oeis.org/A106649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n if n < 10, otherwise a(n) = 10 * a(floor(n/10)) + 9 -
 *            (n mod 10). This is a self‑inverse permutation of the natural
 *            numbers
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 10)
        return (seq_t)n;

    idx_t rest = n / 10;
    uint8_t d = (uint8_t)(n % 10);
    seq_t rest_a = a(rest);

    return rest_a * 10 + (seq_t)(9 - d);
}
