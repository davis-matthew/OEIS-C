/*
 * A171525  Numerator of (n-th noncomposite/n).
 * https://oeis.org/A171525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for 1≤n≤3; a(n) = prime(n‑1) for n≥4
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n <= 3)
        return (seq_t)1;
    return (seq_t)get_prime(n - 1);
}
