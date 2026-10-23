/*
 * A045642  Palindromic and divisible by 7.
 * https://oeis.org/A045642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 113, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   pow10k, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Palindromic numbers divisible by 7 (offset 1).  Sequence A045642. */

#include <stdbool.h>
#include "../utils/pow10k.h"
#include "../utils/reverse_digits.h"

/* Return the decimal reverse of x.  Handles 0 correctly. */

/* Return the n-th term of the sequence, where a(1) = 0. */
seq_t a(idx_t n)
{
    if (n == 1)
        return 0;               /* a(1) */
    idx_t target = n - 1;        /* count of non‑zero palindromes needed */
    idx_t count  = 0;            /* how many divisible palindromes have been found */

    for (idx_t len = 1; ; ++len) {
        idx_t halfLen = (len + 1) / 2;          /* number of digits in the left half */
        seq_t start   = pow10k(halfLen - 1);    /* first half cannot start with 0 */
        seq_t end     = pow10k(halfLen) - 1;    /* inclusive upper bound */

        for (seq_t half = start; half <= end; ++half) {
            seq_t pal;
            if (len % 2 == 0) {                 /* even length */
                pal = half * pow10k(halfLen) + reverse_digits(half);
            } else {                            /* odd length */
                pal = half * pow10k(halfLen - 1) + reverse_digits(half / 10);
            }
            if (pal % 7 == 0) {
                ++count;
                if (count == target)
                    return pal;
            }
        }
    }
}
