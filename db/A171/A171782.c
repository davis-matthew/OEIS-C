/*
 * A171782  Binary expansion of numbers in A171781.
 * https://oeis.org/A171782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007088(A171781(n)) where A171781(n) enumerates binary
 *            strings of length L ≥ 2 with the property that the second bit
 *            equals the last bit, sorted by increasing length L and
 *            lexicographically within each length. a(n) is obtained by
 *            writing that integer in binary and interpreting the binary d...
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 512, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the length L of the binary string for the n-th term.
       For L = 2 there are 2 strings; for L ≥ 3 there are 2^(L‑2) strings. */
    idx_t L = 2;                    /* current length being examined */
    idx_t count = 2;                /* number of strings of current length */
    idx_t rem = n;                  /* remaining index within the whole sequence */

    /* Descend lengths until the remaining index fits in the current block. */
    while (rem > count)
    {
        rem -= count;
        ++L;
        count = (L == 2) ? (idx_t)2
                         : ((idx_t)1 << (L - 2));   /* 2^(L‑2) for L ≥ 3 */
    }
    /* rem now in [1, count]; convert to a 0‑based offset inside this block */
    idx_t offset = rem - (idx_t)1;

    seq_t val;                      /* integer whose binary digits form a(n) */

    if (L == 2)
    {   /* Special case: only two strings 10 and 11 */
        seq_t block = (seq_t)offset;          /* 0 or 1 */
        val = ((seq_t)1 << 1) | block;        /* bits: [1]=1, [0]=block */
    }
    else
    {
        idx_t half_size = ((idx_t)1 << (L - 3));  /* 2^(L‑3) */
        idx_t block = offset / half_size;         /* 0 or 1 */
        idx_t pos   = offset % half_size;         /* middle bits index */

        val = ((seq_t)1 << (L - 1))          /* most significant bit 1 */
              | ((seq_t)block << (L - 2))     /* second bit (and also bit 0) */
              | ((seq_t)pos << 1)            /* middle bits (positions L‑3 … 1) */
              | (seq_t)block;                /* least significant bit (matches second) */
    }

    /* Convert the binary integer `val` into a decimal number whose digits
       are the binary digits of `val`. */
    seq_t result = (seq_t)0;
    for (int i = (int)L - 1; i >= 0; --i)
    {
        seq_t bit = (val >> i) & (seq_t)1;
        result = result * (seq_t)10 + bit;
    }
    return result;
}
