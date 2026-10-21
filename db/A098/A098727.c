/*
 * A098727  Consider the sequence {b(n), n >= 1} of digits of the natural (or
 *          counting) numbers: 1 2 3 4 5 6 7 8 9 1 0 1 1 1 2 1 3 1 4 1 5 1 6 1
 *          7 1 8 1 9 2 0... (A007376); a(n) = b(n) + n.
 * https://oeis.org/A098727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n + b(n) where b(n) is the n-th digit (1‑based) of the
 *            concatenation 1234567891011121314… So a(n) is the n-th term of
 *            A098727
 * Range:     u8 n <= 248, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow10_u64(idx_t e)
{
    seq_t p = 1;
    for (idx_t i = 0; i < e; ++i)
        p *= 10;
    return p;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* outside the sequence, but safe */
    seq_t n_seq = (seq_t)n;

    /* Find the digit b(n) */
    idx_t d = 1;                 /* current digit length of numbers  */
    idx_t remaining = n;         /* digits still to skip             */
    seq_t block_digits;

    while (1) {
        seq_t numbers_in_block = 9 * pow10_u64(d - 1);          /* numbers with d digits   */
        block_digits = numbers_in_block * d;                   /* total digits in block   */
        if (remaining <= block_digits)                         /* our digit is here       */
            break;
        remaining -= (idx_t)block_digits;    /* subtraction fits in idx_t */
        ++d;
    }

    /* Now remaining is 1‑based position within the block of d‑digit numbers */
    seq_t offset = (remaining - 1) / d;   /* number of numbers after the first in block  */
    seq_t number = pow10_u64(d - 1) + offset;

    idx_t digit_index = (remaining - 1) % d;            /* 0‑based from leftmost digit */
    seq_t divisor = pow10_u64(d - 1 - digit_index);
    seq_t digit = (number / divisor) % 10;

    return n_seq + digit;
}
