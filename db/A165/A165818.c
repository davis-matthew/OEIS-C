/*
 * A165818  Largest integer coprime to n whose binary representation is a
 *          substring of the binary representation of n.
 * https://oeis.org/A165818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   Largest integer coprime to n whose binary representation is a *
 *            substring of the binary representation of n
 * Range:     u8 n <= 512, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Euclidean algorithm for the integer type seq_t. */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* safety for n < 1, not used by OEIS */

    seq_t orig = (seq_t)n;            /* keep original value for gcd */
    idx_t tmp = n;
    unsigned len = 0;

    /* Determine bit length of n */
    while (tmp) {
        ++len;
        tmp >>= 1;
    }

    /* Extract binary digits (most significant first) */
    uint8_t bits[64];
    tmp = n;
    for (int i = (int)len - 1; i >= 0; --i) {
        bits[i] = (uint8_t)(tmp & 1);
        tmp >>= 1;
    }

    seq_t max_val = 0;

    /* Enumerate all substrings and keep the largest coprime one */
    for (unsigned start = 0; start < len; ++start) {
        seq_t cur = 0;
        for (unsigned end = start; end < len; ++end) {
            cur = (cur << 1) | (seq_t)bits[end];
            if (seq_gcd(cur, orig) == 1 && cur > max_val)
                max_val = cur;
        }
    }

    return max_val;
}
