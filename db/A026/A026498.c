/*
 * A026498  a(n) = t(1+3n), where t = A001285 (Thue-Morse sequence).
 * https://oeis.org/A026498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + A001285(1+3*n) (Thue‑Morse parity)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t x = 1ULL + 3ULL * (uint64_t)n;
    unsigned parity = 0;
    while (x)
    {
        parity ^= x & 1ULL;
        x >>= 1;
    }
    return (seq_t)(1 + parity);
}
