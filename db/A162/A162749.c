/*
 * A162749  Write the n-th (odd) binary palindrome in binary. If there are an
 *          even number of digits, then combine the middle two digits into one
 *          digit. If there are an odd number of digits, then double the
 *          middle digit. a(n) is the decimal result.
 * https://oeis.org/A162749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = result of taking the n‑th binary palindrome that
 *            represents an odd integer and applying the following
 *            transformation: if its length is even, delete one of the two
 *            middle bits (they are identical because the string is a
 *            palindrome); if its length is odd, duplicate the middle bit. The
 *            result...
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

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
    /* 0‑based index into the list of binary palindromes.  */
    idx_t idx = n - 1;

    /* Find the length L of the nth palindrome.  */
    for (idx_t L = 1;; ++L) {
        idx_t half = (L + 1) >> 1;            /* ceil(L/2)          */
        idx_t count = (idx_t)1 << (half - 1); /* palindromes of this length */

        if (idx < count) {
            /* Build the left half of the palindrome.  */
            seq_t leftHalf = (((seq_t)1) << (half - 1)) | idx;

            if (L & 1) {                     /* odd length */
                seq_t mid = leftHalf & 1;
                seq_t rev = reverse_bits(leftHalf >> 1, half - 1);
                return (leftHalf << half) | (mid << (half - 1)) | rev;
            } else {                         /* even length */
                seq_t rev = reverse_bits(leftHalf, half);
                return (((leftHalf >> 1) << half) | rev);
            }
        }
        idx -= count;                         /* skip this block    */
    }
}
