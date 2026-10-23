/*
 * A104613  Write the natural numbers in base 9 in a triangle with k digits in
 *          the k-th row, as shown below. Sequence gives the leading diagonal.
 * https://oeis.org/A104613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n(n+1)/2‑th digit of the infinite base‑9 digit stream
 *            that results from writing the natural numbers 1,2,3,… in base 9.
 *            The sequence is the leading diagonal of the triangle with k
 *            digits in the k‑th row
 * Range:     u8 n <= 22, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* a(0) is undefined; return 0 for safety */

    /*  position of the required digit in the infinite digit stream (1‑based) */
    seq_t pos = (seq_t)n * ((seq_t)n + 1) / 2;

    /* find the block of numbers whose base‑9 representations have the
       same number of digits (d).  The cumulative number of digits
       processed from shorter blocks is stored in cumulative. */
    idx_t  d          = 1;      /* current digit length in base 9          */
    seq_t  pow9       = 1;      /* 9^(d-1)                                */
    seq_t  cumulative = 0;      /* digits accumulated from shorter blocks */

    while (1) {
        seq_t block_count  = 8 * pow9;      /* numbers in this block   */
        seq_t block_digits = block_count * d; /* total digits contributed */

        if (cumulative + block_digits >= pos)
            break;                 /* the desired digit is in this block */

        cumulative += block_digits;
        ++d;
        pow9 *= 9;                 /* next power of 9           */
    }

    /* position within the current block (1‑based) */
    seq_t offset       = pos - cumulative;
    seq_t number_offset = (offset - 1) / d;                 /* 0‑based index of the number in this block   */
    idx_t digit_index   = (idx_t)((offset - 1) % d);        /* 0‑based index of the digit within that number */

    seq_t start  = pow9;                 /* first decimal number in this block */
    seq_t number = start + number_offset; /* the decimal number whose digit we want */

    /* Extract the base‑9 digits of 'number' (most significant first). */
    uint8_t digits[32];                  /* d will never exceed 32 for the problem limits */
    seq_t temp = number;
    for (idx_t i = d; i > 0; --i) {
        digits[i - 1] = (uint8_t)(temp % 9);
        temp /= 9;
    }

    return (seq_t)digits[digit_index];
}
