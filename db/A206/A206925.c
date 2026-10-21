/*
 * A206925  Number of contiguous palindromic bit patterns in the binary
 *          representation of n.
 * https://oeis.org/A206925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of overlapping palindromic substrings in the
 *            binary representation of n; computed by two‑center expansion
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handles n >= 1, as required by the sequence offset. */
    if (n == 0)
        return 0;

    /* Determine bit length L = floor_log2(n) + 1. */
    idx_t L = 0;
    idx_t tmp = n;
    while (tmp) {
        ++L;
        tmp >>= 1;
    }

    /* Extract bits into a small array: bits[0] is the most significant bit. */
    unsigned char bits[64];
    idx_t i;
    for (i = 0; i < L; ++i) {
        bits[i] = (unsigned char)((n >> (L - 1 - i)) & 1);
    }

    seq_t count = 0;

    /* Count odd‑length palindromes centered at each bit. */
    for (i = 0; i < L; ++i) {
        idx_t left = i, right = i;
        while (right < L) {
            if (bits[left] != bits[right])
                break;
            ++count;
            if (left == 0)
                break;
            --left;
            ++right;
        }
    }

    /* Count even‑length palindromes centered between bits. */
    for (i = 0; i + 1 < L; ++i) {
        idx_t left = i, right = i + 1;
        while (right < L) {
            if (bits[left] != bits[right])
                break;
            ++count;
            if (left == 0)
                break;
            --left;
            ++right;
        }
    }

    return count;
}
