/*
 * A118749  Largest prime <= 3*n.
 * https://oeis.org/A118749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest prime <= 3*n
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; behavior for n == 0 is undefined */
    seq_t limit = (seq_t)n * 3;
    idx_t x = (idx_t)limit;
    while (!is_prime(x)) {
        --x;
    }
    return (seq_t)x;
}
