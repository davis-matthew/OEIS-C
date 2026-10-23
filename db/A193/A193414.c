/*
 * A193414  Numbers m such that written in base 2 the structure of digits
 *          represents a valley.
 * https://oeis.org/A193414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Sequence A193414: numbers whose binary representation starts
   and ends with 1 and contains at most one contiguous block of zeros. */

seq_t a(idx_t n)
{
    /* n is 1‑based. For n == 0 the value is undefined; return 0. */
    if (n == 0)
        return 0;

    /* Find the binary length L of the n‑th term. */
    idx_t cum = 0;   /* cumulative elements up to previous length */
    idx_t L   = 1;   /* current length (number of bits) */
    idx_t cnt;      /* number of terms of length L */

    while (1) {
        if (L == 1)
            cnt = 1;  /* only "1" */
        else
            cnt = 1 + ((L - 2) * (L - 1)) / 2;   /* 1 all‑ones + zero‑block patterns */

        if (cum + cnt >= n)
            break;   /* n lies in the current length block */

        cum += cnt;
        ++L;
    }

    /* Index of the desired term within the block of length L (0‑based). */
    idx_t k = n - cum - 1;

    /* If k corresponds to the all‑ones value, return it directly. */
    if (k == cnt - 1) {
        return (((seq_t)1 << L) - 1);
    }

    /* Otherwise we need the k‑th zero‑block pattern.
       Patterns are enumerated in increasing numeric order by:
           i from 1 to L-2 (start of block counted from the most significant side)
           j from L-2 down to i (end of block)   */
    idx_t zero_idx = k;   /* zero‑based index among the zero‑block patterns */

    for (idx_t i = 1; i <= L - 2; ++i) {
        for (idx_t j = L - 2; j >= i; --j) {
            if (zero_idx == 0) {
                /* Compute the value with a zero block from bit positions
                   L-1-j ... L-1-i (inclusive). */
                seq_t val = (((seq_t)1 << L) - 1);   /* all ones */

                /* Subtract the bits of the zero block */
                for (idx_t b = L - 1 - j; b <= L - 1 - i; ++b)
                    val -= (((seq_t)1 << b));

                return val;
            }
            --zero_idx;
        }
    }

    /* The following line should never be reached for a valid, non‑zero n. */
    return (((seq_t)1 << L) - 1);
}
