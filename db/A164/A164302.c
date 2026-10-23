/*
 * A164302  a(n) = 2* (the n-th positive (odd) integer that is a palindrome
 *          when written in base 2).
 * https://oeis.org/A164302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 2 * (n-th odd binary palindrome)
 * Range:     u8 n <= 22, u16 n <= 382, u32 n <= 9723, u64 n <= 9723,
 *            u128 n <= 9723, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Reverse the lowest 'bits' bits of x. */
static seq_t reverse_bits(seq_t x, unsigned bits)
{
    seq_t rev = 0;
    while (bits--)
    {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, guard anyway */

    idx_t remaining = n;
    idx_t len = 1;

    /* Find the length of the n-th binary palindrome. */
    while (1)
    {
        idx_t count;
        if (len == 1)
            count = 1;
        else if (len & 1)            /* odd length */
        {
            idx_t k = (len - 1) / 2;
            count = ((idx_t)1 << k);        /* 2^k palindromes */
        }
        else                         /* even length */
        {
            idx_t k = len / 2;
            count = ((idx_t)1 << (k - 1));   /* 2^(k-1) palindromes */
        }

        if (remaining <= count)
            break;

        remaining -= count;
        ++len;
    }

    seq_t palindrome;

    if (len == 1)
    {
        palindrome = 1;                 /* only '1' */
    }
    else if (len & 1)                   /* odd length */
    {
        idx_t k = (len - 1) / 2;        /* number of bits in the left half */
        idx_t idx = remaining - 1;      /* 0‑based index within this length */

        idx_t leftHalfIdx = idx >> 1;   /* index of left half */
        seq_t middle = (seq_t)(idx & 1);

        seq_t leftHalf = ((seq_t)1 << (k - 1)) + (seq_t)leftHalfIdx;
        seq_t rev = reverse_bits(leftHalf, (unsigned)k);

        palindrome = (leftHalf << (k + 1)) | (middle << k) | rev;
    }
    else                                 /* even length */
    {
        idx_t k = len / 2;
        idx_t leftHalfIdx = remaining - 1;

        seq_t leftHalf = ((seq_t)1 << (k - 1)) + (seq_t)leftHalfIdx;
        seq_t rev = reverse_bits(leftHalf, (unsigned)k);

        palindrome = (leftHalf << k) | rev;
    }

    return palindrome * 2;              /* a(n) = 2 * palindrome */
}
