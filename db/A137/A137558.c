/*
 * A137558  A137521(n)/5.
 * https://oeis.org/A137558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = 20p(n) + 9, where p(n) is the n‑th prime
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for indices below the offset */

    uint32_t p = get_prime(n);           /* n‑th prime (uint32_t) */
    return (seq_t)((seq_t)20 * (seq_t)p + (seq_t)9);
}
