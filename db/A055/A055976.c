/*
 * A055976  Remainder when (n-1)! + 1 is divided by n.
 * https://oeis.org/A055976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n-1)! + 1) mod n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) = ((n-1)! + 1) mod n for n >= 1.
 * Uses modular arithmetic to keep intermediate values bounded.
 */
seq_t a(idx_t n)
{
    if (n <= 1)                /* handles n = 1 (edge case) */
        return (seq_t)0;

    uint64_t res = 1;          /* (n-1)! modulo n, stored as uint64_t */
    for (idx_t i = 2; i < n; ++i) {
        unsigned __int128 temp = (unsigned __int128)res * i;
        res = (uint64_t)(temp % (unsigned __int128)n);
    }

    uint64_t val = (res + 1) % n;
    return (seq_t)val;
}
