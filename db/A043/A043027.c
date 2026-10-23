/*
 * A043027  Base-8 palindromes that start with 7.
 * https://oeis.org/A043027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Enumeration of base‑8 palindromes that start with 7. The n‑th
 *            term is the n‑th number (in increasing order) whose base‑8
 *            representation is a palindrome and begins with digit 7
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note : idx_t and seq_t are type aliases supplied by the build
          and are not defined here. */

#include <stdbool.h>
#include <stdint.h>

/* Helper to compute the n‑th term */
seq_t a(idx_t n)
{
    /* Handle index < 1: undefined, but we simply return 0 */
    if (n < 1) return (seq_t)0;

    /* Find the block (m) containing the n-th term.
       For each m>=1 there are 2*8^(m-1) palindromes:
       8^(m-1) of length 2m-1 (odd) and 8^(m-1) of length 2m (even). */
    idx_t cum   = 0;          /* number of terms before current block */
    idx_t pow8  = 1;          /* 8^(m-1) for current block */
    int   m     = 1;          /* block index (m>=1) */

    while (1)
    {
        /* If the current 8^(m-1) itself is already larger than n,
           the desired term must lie in this block. */
        if (pow8 > n)
            break;        /* block contains the answer */

        /* Avoid overflow of 2*pow8.  If the multiplication would
           overflow the unsigned type, the block size is larger
           than any possible remaining index. */
        if (pow8 > (idx_t)-1 / 2)
            break;

        idx_t block_size = 2 * pow8;   /* terms in this block */

        if (n <= cum + block_size)
            break;        /* answer is in this block */

        cum += block_size;
        /* Prepare pow8 for the next block: multiply by 8 with
           a safe multiplication (we know pow8 <= (2^64-1)/8 here). */
        pow8 *= 8;
        ++m;
    }

    /* Determine whether the term lies in the odd (length 2m-1)
       or even (length 2m) part of this block. */
    idx_t k = n - cum;            /* 1‑based index inside this block */
    idx_t local = (k - 1) % pow8; /* 0‑based index inside the half‑group */

    /* Special handling for the very first block (m==1). */
    if (m == 1) {
        return (k <= 1) ? (seq_t)7 : (seq_t)63;
    }

    /* Build the free digit part (length m-1) in an array.
       The most significant free digit is at index 0. */
    unsigned char free_digits[64] = {0};
    int len_half = m - 1;
    idx_t rem = local;
    for (int i = len_half - 1; i >= 0; --i) {
        free_digits[i] = (unsigned char)(rem % 8);
        rem /= 8;
    }

    /* Construct the palindrome by iterating over its digits
       and accumulating the value in base 8. */
    seq_t val = 7; /* most significant digit d1 */

    if (k <= pow8) {           /* odd length: 2m-1 */
        /* forward free digits */
        for (int i = 0; i < len_half; ++i)
            val = val * 8 + free_digits[i];
        /* reverse without the central digit */
        for (int i = len_half - 2; i >= 0; --i)
            val = val * 8 + free_digits[i];
        /* trailing digit */
        val = val * 8 + 7;
    } else {                   /* even length: 2m */
        /* forward free digits */
        for (int i = 0; i < len_half; ++i)
            val = val * 8 + free_digits[i];
        /* reverse including all free digits */
        for (int i = len_half - 1; i >= 0; --i)
            val = val * 8 + free_digits[i];
        /* trailing digit */
        val = val * 8 + 7;
    }

    return val;
}
