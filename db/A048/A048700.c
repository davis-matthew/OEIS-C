/*
 * A048700  Binary palindromes of odd length (written in base 10).
 * https://oeis.org/A048700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A048700: Binary palindromes of odd length (decimal)
 *  a(n) is the n-th smallest positive integer whose binary representation
 *  is an odd‑length palindrome.  The function computes the k‑th palindrome
 *  directly from n, without any external utilities.
 */

#include <stdint.h>
#include <stddef.h>

/* Return the n‑th binary palindrome of odd length (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined in the OEIS offset; return 0 for safety */
        return (seq_t)0;

    /* Find the group k such that the cumulative count up to k
     * (inclusive) is at least n.  Group k contains 2^k palindromes
     * of length 2*k+1.  The cumulative count up to k is 2^(k+1)-1. */
    idx_t k = 0;
    while (((uint64_t)1 << (k + 1)) - 1 < n)
        ++k;

    /* Count of palindromes before this group */
    idx_t prev_count = ((uint64_t)1 << k) - 1;
    /* Position within the current group (1 .. 2^k) */
    idx_t offset = n - prev_count;
    /* Left part of the palindrome: k+1 bits starting with a 1.
     * It ranges from 2^k to 2^(k+1)-1 inclusive. */
    idx_t left = ((uint64_t)1 << k) + (offset - 1);

    /* Reverse the lower k bits of left >> 1 (the middle bit is omitted). */
    seq_t rev = 0;
    idx_t tmp = left >> 1;
    for (idx_t i = 0; i < k; ++i) {
        rev = (rev << 1) | (tmp & 1);
        tmp >>= 1;
    }

    /* Assemble the full palindrome: left (k+1 bits) followed by rev. */
    seq_t pal = (((seq_t)left) << k) | rev;
    return pal;
}
