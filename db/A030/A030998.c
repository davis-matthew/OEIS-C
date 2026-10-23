/*
 * A030998  Write n in base 7 and juxtapose.
 * https://oeis.org/A030998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, cons, easy, tabf
 * Formula:   a(n) is the nth digit (index 0) of the base‑7 Champernowne
 *            constant * (the concatenation of the base‑7 representations of
 *            0,1,2,…)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)          /* the very first digit is 0 */
        return (seq_t)0;

    n--;                  /* skip the leading '0' */

    idx_t pow7 = 1;       /* 7^(d-1) for the current block of d‑digit numbers */
    idx_t d    = 1;       /* current number of base‑7 digits */

    /* Find the block (digit length) that contains the requested digit */
    while (1) {
        /* count of numbers with exactly d digits: 6 * 7^(d-1) */
        idx_t count_num = 6 * pow7;
        /* total digits contributed by this block */
        idx_t digits_block = count_num * d;

        if (n < digits_block)
            break;        /* the desired digit lies in this block */

        n -= digits_block;    /* skip this block */
        pow7 *= 7;            /* move to next digit length */
        d++;
    }

    /* Position within the block */
    idx_t block_index = n / d;          /* which number in the block (0‑based) */
    idx_t digit_pos   = n % d;          /* which digit of that number (0‑msb) */

    idx_t number = pow7 + block_index;  /* the actual integer whose base‑7 form is needed */

    /* Extract the base‑7 digits of 'number' into an array (msb first) */
    seq_t digits[64];  /* safe for all d that fit in 64‑bit indices */
    for (idx_t i = d; i > 0; ) {
        i--;
        digits[i] = (seq_t)(number % 7);
        number /= 7;
    }

    return digits[digit_pos];
}
