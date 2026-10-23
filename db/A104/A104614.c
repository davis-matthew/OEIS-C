/*
 * A104614  Write the natural numbers in base 11 in a triangle with k digits
 *          in the k-th row, as shown below. Sequence gives the leading
 *          diagonal.
 * https://oeis.org/A104614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) equals the base‑11 digit at position n(n+1)/2 in the
 *            concatenation of the base‑11 representations of the natural
 *            numbers starting with 1
 * Range:     u8 n <= 22, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Compute the absolute position of the desired digit */
    seq_t pos = ((seq_t)n * ((seq_t)n + 1)) / 2;

    /* Find the block of numbers that contains this position.  Each
       block consists of all L‑digit numbers in base 11.  The count of
       such numbers is 10 * 11^(L-1).  The total number of digits
       contributed by this block is block_digits = count * L. */
    seq_t rem = pos;                /* remaining position within the search */
    seq_t pow = 1;                  /* current 11^(L-1); starts with 11^0 */
    size_t L = 1;                   /* current digit length */

    for (;; ++L) {
        seq_t count   = (seq_t)10 * pow;        /* 10 * 11^(L-1) */
        seq_t block   = count * (seq_t)L;      /* digits contributed by this block */

        if (rem <= block)                    /* desired position falls here */
            break;

        rem -= block;                        /* skip this block */
        pow *= (seq_t)11;                    /* move to next power of 11 */
    }

    /* 'rem' is now 1‑indexed within the block of L‑digit numbers.*/
    seq_t offset        = rem - 1;            /* 0‑indexed offset in block */
    seq_t index_in_block = offset / (seq_t)L; /* which number in this block */
    seq_t digit_index    = offset % (seq_t)L; /* which digit of that number */

    /* The first number of this block is 11^(L-1).  Add the offset to get
       the actual integer to be represented in base 11. */
    seq_t number = pow + index_in_block;

    /* Extract the requested digit.  Digits are stored most‑significant first.
       Compute the divisor for the digit: 11^(L-1-digit_index). */
    seq_t divisor = pow;
    for (seq_t i = 0; i < digit_index; ++i)
        divisor /= (seq_t)11;

    seq_t digit = (number / divisor) % (seq_t)11;
    return digit;
}
