/*
 * A175910  Take the left or right binary concatenation of the numbers 1 to n,
 *          whichever is greater, delete digits identical to corresponding
 *          digits in the other concatenation, condense the remaining digits,
 *          and convert to decimal.
 * https://oeis.org/A175910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = value obtained by taking the greater of the leftward and
 *            rightward binary concatenations of the integers 1..n, deleting
 *            digits identical to the corresponding digits in the other
 *            concatenation, condensing the remaining digits, and interpreting
 *            the result as a binary number
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* total number of bits in the concatenations */
    idx_t total_bits = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t t = k;
        idx_t len = 0;
        while (t) { ++len; t >>= 1; }
        total_bits += len;
    }

    unsigned char *left  = (unsigned char *)malloc(total_bits);
    unsigned char *right = (unsigned char *)malloc(total_bits);
    if (!left || !right) {
        free(left);
        free(right);
        return (seq_t)0;            /* allocation failed */
    }

    /* build left concatenation (1,2,...,n) */
    idx_t pos = 0;
    for (idx_t k = 1; k <= n; ++k) {
        int msb = 0;
        idx_t t = k;
        while (t >>= 1) ++msb;           /* msb = floor(log2(k)) */
        for (int bit = msb; bit >= 0; --bit) {
            left[pos++] = (unsigned char)((k >> bit) & 1);
        }
    }

    /* build right concatenation (n,...,2,1) */
    pos = 0;
    for (idx_t k = n; k > 0; --k) {
        int msb = 0;
        idx_t t = k;
        while (t >>= 1) ++msb;
        for (int bit = msb; bit >= 0; --bit) {
            right[pos++] = (unsigned char)((k >> bit) & 1);
        }
    }

    /* find first differing bit from the most significant side */
    idx_t diff = 0;
    while (diff < total_bits && left[diff] == right[diff]) ++diff;

    /* determine which concatenation is greater */
    unsigned char *greater = left;
    unsigned char *other   = right;
    if (diff < total_bits && left[diff] == 0) {   /* right is greater */
        greater = right;
        other   = left;
    }

    /* construct the resulting binary number from differing bits */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < total_bits; ++i) {
        if (greater[i] != other[i]) {
            result = result * 2 + greater[i];
        }
    }

    free(left);
    free(right);
    return result;
}
