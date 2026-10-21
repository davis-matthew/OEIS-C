/*
 * A206914  Least binary palindrome >= n; the binary palindrome ceiling
 *          function.
 * https://oeis.org/A206914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least binary palindrome ≥ n (base 2)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Reverse the lowest `bits` bits of `x`. */
static seq_t reverse_bits(seq_t x, idx_t bits)
{
    seq_t rev = 0;
    for (idx_t i = 0; i < bits; ++i) {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    /* bit length of x; a(0) becomes 0 instantly */
    idx_t L = bitlen_seq_t(x);
    while (1) {
        idx_t half = (L + 1) / 2;          /* bits in left half (middle included if odd) */
        seq_t left = x >> (L - half);      /* left half (with middle if odd) */

        seq_t palindrome;
        if (L % 2 == 0) {                   /* even length */
            seq_t rev = reverse_bits(left, half);
            palindrome = (left << half) | rev;
        } else {                            /* odd length */
            seq_t rev = reverse_bits(left >> 1, half - 1);
            palindrome = (left << (half - 1)) | rev;
        }

        if (palindrome >= x)
            return palindrome;             /* ceiling reached */

        /* Need a larger palindrome of the same length. Increment left. */
        left++;
        /* If left exceeds its bit-width, we must increase the length. */
        if (left >> half != 0) {
            /* Minimal palindrome of the next length: 1 followed by L‑1 zeros and 1. */
            return ((seq_t)1 << L) | 1;
        }

        /* Build the palindrome with the incremented left part. */
        if (L % 2 == 0) {
            seq_t rev = reverse_bits(left, half);
            palindrome = (left << half) | rev;
        } else {
            seq_t rev = reverse_bits(left >> 1, half - 1);
            palindrome = (left << (half - 1)) | rev;
        }
        return palindrome;                  /* this is now ≥ n */
    }
}
