/*
 * A098733  Consider the sequence {b(n), n >= 1} of digits of the natural (or
 *          counting) numbers: 1 2 3 4 5 6 7 8 9 1 0 1 1 1 2 1 3 1 4 1 5 1 6 1
 *          7 1 8 1 9 2 0... (A007376); a(n) = n*b(n).
 * https://oeis.org/A098733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n * d
 *   where d is the n-th digit in the infinite concatenation
 *   of the natural numbers: 1,2,3,4,5,6,7,8,9,10,11,12,...
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the block of numbers having the same number of digits
     * that contains the desired digit.  For block 'd' (d=1,2,…) the
     * block starts at 10^(d-1) and contains 9·10^(d-1) numbers, each
     * contributing d digits.  */
    idx_t digit_len = 1;          /* number of digits of numbers in this block */
    idx_t pow10     = 1;          /* 10^(digit_len-1) – the first number of the block */
    idx_t cumulative = 0;         /* digits counted before the current block */

    while (1) {
        idx_t block_count  = 9 * pow10;                 /* numbers in the block   */
        idx_t block_digits = block_count * digit_len;   /* digits contributed     */
        if (cumulative + block_digits >= n)            /* the wanted digit lies
                                                       * in this block        */
            break;
        cumulative += block_digits;
        pow10 *= 10;            /* move to next digit length */
        digit_len++;
    }

    /* Position of the desired digit within the block (0‑based) */
    idx_t offset         = n - cumulative - 1;
    idx_t number_index   = offset / digit_len;          /* which number in block */
    idx_t offset_in_num  = offset % digit_len;          /* position in the number */

    idx_t number = pow10 + number_index;                /* the actual number */

    /* Extract the required digit.  Find the divisor 10^(digit_len-1-offset_in_num). */
    idx_t divisor = 1;
    for (idx_t i = 0; i < digit_len - 1 - offset_in_num; i++)
        divisor *= 10;

    idx_t digit = (number / divisor) % 10;

    /* Final result: n times that digit.  All arithmetic uses the
     * provided integer types; no floating point is used. */
    seq_t result = (seq_t)n * (seq_t)digit;
    return result;
}
