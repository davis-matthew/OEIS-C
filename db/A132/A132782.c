/*
 * A132782  Numbers with at least one occurrence of '1010' in binary
 *          representation.
 * https://oeis.org/A132782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n-th positive integer whose binary representation
 * contains the substring 1010. (OEIS A132782) */
#include <stdint.h>
#include <stdbool.h>

/* Check whether a number has the binary substring 1010. */
static bool has_1010(seq_t x)
{
    uint64_t v = (uint64_t)x;
    if (v < 10) {                    /* smallest number with 1010 is 10 */
        return false;
    }
    /* Find index of most significant bit. */
    int msb = 0;
    uint64_t t = v;
    while (t >> 1u) {
        t >>= 1u;
        msb++;
    }
    uint16_t win = 0;
    for (int i = msb; i >= 0; --i) {
        win = ((win << 1u) | ((v >> i) & 1u)) & 0xFu;
        if (win == 10u) {
            return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* Undefined for n = 0; return 0 for safety. */
    }
    idx_t count = 0;
    uint64_t u = 1;                       /* use 64‑bit variable for iteration */
    while (count < n) {
        if (has_1010((seq_t)u)) {
            ++count;
            if (count == n) {
                return (seq_t)u;
            }
        }
        ++u;
    }
    return 0; /* unreachable, keeps compiler happy */
}
