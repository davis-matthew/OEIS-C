/*
 * A043272  Sum of the digits of the n-th base 13 palindrome.
 * https://oeis.org/A043272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = sum of the digits of the n‑th base‑13 palindrome
 * (OEIS sequence A043272).  The index n is 1‑based; a(1)=0, a(2)=1, … */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first 13 single‑digit palindromes: 0 … 12 */
    if (n <= 13)
        return (seq_t)(n - 1);

    /* Remaining length ≥ 2 */
    idx_t rem = n - 13;          /* number of palindromes after the 13 singles */
    idx_t length = 2;            /* current palindrome length in base‑13 */

    while (1) {
        /* half = ceil(length/2) */
        idx_t half = (length + 1) / 2;

        /* number of palindromes of this length: 12*13^(half‑1) */
        idx_t pow13 = 1;
        for (idx_t i = 1; i < half; ++i)
            pow13 *= 13;
        idx_t count = 12 * pow13;

        if (rem <= count) /* n lies in this block */
            break;

        rem -= count;
        ++length;
    }

    /* Convert the 1‑based index within this block to 0‑based */
    idx_t index = rem - 1;

    /* Determine the digit pattern of the palindrome */
    idx_t half = (length + 1) / 2;

    /* Pre‑allocate temporary digit buffer; half ≤ 4 for n ≤ 10000 */
    uint8_t digits[8] = {0};

    /* Leading (most significant) digit: 1‑based to avoid leading 0 */
    idx_t pow13 = 1;
    for (idx_t i = 1; i < half; ++i)
        pow13 *= 13;

    digits[0] = (uint8_t)(1 + index / pow13);
    idx_t rem_idx = index % pow13;

    /* Remaining digits (if any) are the lower half of the number */
    for (idx_t i = half - 1; i >= 1; --i) {
        digits[i] = (uint8_t)(rem_idx % 13);
        rem_idx /= 13;
    }

    /* Compute the digit sum taking mirror symmetry into account */
    seq_t sum = 0;
    for (idx_t i = 0; i < half; ++i) {
        seq_t d = digits[i];
        if (i != length - 1 - i) /* mirrored digit */
            sum += d * 2;
        else
            sum += d;
    }

    return sum;
}
