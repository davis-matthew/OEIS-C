/*
 * A031235  Triangle T(n,k): write n in base 5, reverse order of digits.
 * https://oeis.org/A031235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf, less
 * Formula:   a(n) is the n-th digit (0‑based) produced by writing every
 *            integer m ≥ 0 in base 5, reversing the order of its digits, and
 *            concatenating those digits in order. Equivalently, a(n) = (m /
 *            5^p) mod 5 where (m, p) are obtained by expressing the index n
 *            in the mixed radix of digit‑counts of base‑5 num...
 * Range:     u8 n <= 948, u16 n <= 4224, u32 n <= 4224, u64 n <= 4224,
 *            u128 n <= 4224, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* first digit is 0 (m=0) */

    /* After the first number (0) we remove its one digit. */
    idx_t rem = n - 1;              /* index into the rest of the sequence */

    idx_t d     = 0;                /* digits of numbers in current block minus one */
    idx_t start = 1;                /* first number in current block (5^d) */
    idx_t powd  = 1;                /* 5^d */

    while (1) {
        idx_t powd1   = powd * 5;                     /* 5^(d+1) */
        idx_t count   = powd1 - powd;                 /* numbers with d+1 digits */
        idx_t digits  = count * (d + 1);              /* digits contributed by block */

        if (rem < digits) break;                     /* index lies in this block */

        rem   -= digits;                             /* skip whole block */
        d     += 1;
        start  = powd1;
        powd   = powd1;
    }

    idx_t offset = rem / (d + 1);                    /* which number in block */
    idx_t p      = rem % (d + 1);                    /* position in that number */

    idx_t m = start + offset;                        /* actual number */

    /* Compute 5^p */
    idx_t powp = 1;
    for (idx_t i = 0; i < p; ++i) powp *= 5;

    /* The digit at position p (0 = least significant) */
    seq_t digit = (seq_t)(m / powp) % 5;
    return digit;
}
