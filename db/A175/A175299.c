/*
 * A175299  a(n) = the smallest positive integer such that A175298(a(n)) = the
 *          n-th positive integer that is a palindrome when written in binary.
 * https://oeis.org/A175299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 2^floor(log2(P)) + (P mod 2^(floor(log2(P)/2)+1)) - 1,
 *            where P is the n‑th binary palindrome
 * Range:     u8 n <= 30, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 1. Find the length of the binary palindrome */
    idx_t remaining = n;
    idx_t len = 0;
    for (len = 1;; ++len) {
        idx_t half = (len + 1) / 2;                   /* ceil(len/2) */
        idx_t cnt = ((idx_t)1) << (half - 1);          /* number of palindromes of this length */
        if (remaining <= cnt) break;
        remaining -= cnt;
    }

    /* 2. Build the n-th palindrome of that length */
    idx_t offset = remaining - 1;                     /* zero‑based offset within this length */
    idx_t halfLen   = (len + 1) / 2;                   /* bits in the left half */
    idx_t bitsInHalf = halfLen - 1;                    /* bits after the leading 1 */
    seq_t left = (((seq_t)1) << bitsInHalf) | (seq_t)offset;

    idx_t revBits  = len - halfLen;                    /* bits to mirror */
    seq_t revBase = left >> (len % 2);                 /* drop middle bit if odd length */

    seq_t rev = 0;
    seq_t tmp = revBase;
    for (idx_t i = 0; i < revBits; ++i) {
        rev = (rev << 1) | (tmp & 1);
        tmp >>= 1;
    }

    seq_t P = (left << revBits) | rev;                /* the n‑th binary palindrome */

    /* 3. Compute 2^floor(log2(P)) */
    seq_t pow2 = 1;
    idx_t exp = 0;
    while ((pow2 << 1) <= P) {
        pow2 <<= 1;
        exp++;
    }

    /* 4. Compute modulus base: 2^(floor(log2(P)/2)+1) */
    seq_t modBase = ((seq_t)1) << (exp / 2 + 1);

    /* 5. Final formula */
    seq_t result = pow2 + (P % modBase) - 1;
    return result;
}
