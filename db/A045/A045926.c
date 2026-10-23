/*
 * A045926  All digits even and nonzero.
 * https://oeis.org/A045926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 980, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: All decimal numbers composed of the digits 2,4,6,8 in increasing order. */
/* The n-th term is obtained by interpreting n‑1 in a mixed radix system:
   groups of length L contain 4^L numbers; within a group the offset is expressed
   in base‑4 and each base‑4 digit d is mapped to the decimal digit 2*d+2. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the length L of the n-th term. */
    idx_t remaining = n;
    idx_t power = 4;   /* 4^1 */
    idx_t L = 1;

    while (remaining > power)
    {
        remaining -= power;
        if (power > (UINT64_MAX >> 2))
        {
            /* Next group would overflow 64‑bit; length is L+1. */
            L++;
            break;
        }
        power <<= 2;  /* multiply by 4 */
        L++;
    }

    /* Zero‑based offset within the group of length L. */
    idx_t offset = remaining - 1;

    /* Convert offset to base‑4 digits (exactly L digits). */
    unsigned char digits[64];  /* sufficient for any L that fits in uint64_t */
    for (idx_t i = L; i > 0; --i)
    {
        digits[i - 1] = (unsigned char)(offset % 4);
        offset /= 4;
    }

    /* Reconstruct the decimal number using mapped digits 2,4,6,8. */
    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i)
    {
        val = val * 10 + (seq_t)(digits[i] * 2 + 2);
    }
    return val;
}
