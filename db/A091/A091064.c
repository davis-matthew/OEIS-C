/*
 * A091064  In binary representation: length of longest proper prefix of n,
 *          that is also a suffix.
 * https://oeis.org/A091064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the length of the longest proper prefix of the binary
 *            representation of n that is also a suffix (i.e., the longest
 *            border of the binary string, excluding the whole string)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 118 terms through n = 8827 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: compute the longest border length of a binary string represented
   by the array bits[0..len-1], where bits[0] is the most significant bit. */
static inline unsigned calc_border(const unsigned char *bits, unsigned len)
{
    /* failure function array for KMP */
    int fail[65];
    fail[0] = 0;
    for (unsigned i = 1; i < len; ++i) {
        int j = fail[i - 1];
        while (j > 0 && bits[i] != bits[j]) j = fail[j - 1];
        if (bits[i] == bits[j]) ++j;
        fail[i] = j;
    }
    return (unsigned)fail[len - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* binary representation "0" → no proper border */

    /* length in bits of n (>=1) */
    unsigned len = 64 - __builtin_clzll((uint64_t)n);
    unsigned char bits[64];                /* up to 64-bit numbers */

    for (unsigned i = 0; i < len; ++i)
        bits[i] = (unsigned char)((n >> (len - 1 - i)) & 1ULL);

    return (seq_t)calc_border(bits, len);
}
