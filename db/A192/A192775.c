/*
 * A192775  The numbers n^2 as n runs through the numbers which are
 *          palindromes in base 2.
 * https://oeis.org/A192775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = p(n-1)^2 where p(k) is the k-th positive integer whose
 *            binary representation is a palindrome; a(1)=0 corresponds to
 *            p(0)=0
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* reverse the lowest 'len' bits of x */
static seq_t reverse_bits(seq_t x, idx_t len)
{
    seq_t r = 0;
    while (len--) {
        r = (r << 1) | (x & 1);
        x >>= 1;
    }
    return r;
}

/* number of binary palindromes of bit‑length L (L>=1) */
static idx_t count_len(idx_t L)
{
    idx_t half = (L + 1) / 2;          /* number of independent bits         */
    return ((idx_t)1) << (half - 1);   /* leading bit must be 1 */
}

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition (the square of 0) */
    if (n == 1) return (seq_t)0;

    idx_t k = n - 1;                    /* k-th positive binary palindrome (k>=1) */
    idx_t acc = 0;                      /* number of palindromes processed so far */
    idx_t L = 1;                        /* current bit-length */

    /* find the bit‑length that contains the k‑th palindrome */
    while (true) {
        idx_t cnt = count_len(L);
        if (acc + cnt >= k) break;
        acc += cnt;
        ++L;
    }

    idx_t offset = k - acc - 1;          /* 0‑based index within length L */

    idx_t halfLen = (L + 1) / 2;
    seq_t half = ((seq_t)1 << (halfLen - 1)) | (seq_t)offset;

    seq_t value;
    if (L & 1) { /* odd length */
        seq_t rev = reverse_bits(half >> 1, halfLen - 1);
        value = (half << (halfLen - 1)) | rev;
    } else {     /* even length */
        seq_t rev = reverse_bits(half, halfLen);
        value = (half << halfLen) | rev;
    }

    return value * value;
}
