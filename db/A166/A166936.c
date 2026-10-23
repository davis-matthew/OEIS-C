/*
 * A166936  Positive integers n such that when they are written in binary, the
 *          longest palindromic substring (or at least one of which that is
 *          tied for longest) in binary n begins and ends with a 0.
 * https://oeis.org/A166936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A166936: positive integers whose longest binary palindrome substring
   starts and ends with 0.  Computed by a brute‑force scan of integers. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bitlen_seq_t.h"

/* Test whether the given number satisfies the property for A166936. */
static bool is_valid_seq_t(seq_t x)
{
    /* Binary representation must have length >= 1.  1 does not qualify. */
    if (x < (seq_t)2) {
        return false;
    }

    /* Length of the binary representation. */
    unsigned len = bitlen_seq_t(x);

    /* Extract the bits into an array (most significant bit first). */
    unsigned char *bits = (unsigned char *)malloc(len);
    if (bits == NULL) {
        /* Should never happen in normal use. */
        return false;
    }
    for (unsigned i = 0; i < len; ++i) {
        bits[i] = ((x >> (len - 1 - i)) & (seq_t)1) ? 1 : 0;
    }

    /* Find the longest palindromic substring and check if any of that
       length starts and ends with 0. */
    unsigned maxLen = 0;
    bool zeroEdge   = false;

    for (unsigned center = 0; center < len; ++center) {
        /* Odd length palindromes. */
        int left  = center;
        int right = center;
        while (left >= 0 && right < (int)len && bits[left] == bits[right]) {
            unsigned curLen = right - left + 1;
            bool z          = (bits[left] == 0 && bits[right] == 0);
            if (curLen > maxLen) {
                maxLen  = curLen;
                zeroEdge = z;
            } else if (curLen == maxLen && z) {
                zeroEdge = true;
            }
            --left;
            ++right;
        }

        /* Even length palindromes. */
        left  = center;
        right = center + 1;
        while (left >= 0 && right < (int)len && bits[left] == bits[right]) {
            unsigned curLen = right - left + 1;
            bool z          = (bits[left] == 0 && bits[right] == 0);
            if (curLen > maxLen) {
                maxLen  = curLen;
                zeroEdge = z;
            } else if (curLen == maxLen && z) {
                zeroEdge = true;
            }
            --left;
            ++right;
        }
    }

    free(bits);
    return zeroEdge;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t val   = 0;

    while (count < n) {
        ++val;
        if (is_valid_seq_t(val)) {
            ++count;
        }
    }
    return val;
}
