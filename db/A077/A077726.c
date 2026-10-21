/*
 * A077726  Smallest number beginning with n and having a digit sum n.
 * https://oeis.org/A077726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the smallest number that starts with the decimal
 *            representation of n and has digit sum equal to n
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   The algorithm:
   - Let s be the sum of the digits of n.
   - Let d = n - s > 0.
   - The minimal number of extra digits needed to reach the required sum
     is ceil(d/9), because each new digit can contribute at most 9.
   - Let k = ceil(d/9). Then form the suffix consisting of
     (d - 9*(k-1)) followed by k-1 digits of 9; this achieves the exact
     sum and is the smallest possible suffix of length k.
   - Append this suffix to the prefix n:  a(n) = n * 10^k + suffix.
*/

seq_t a(idx_t n)
{
    /* Single‑digit n is trivially the answer. */
    if (n < 10)
        return (seq_t)n;

    /* Compute the digit sum of n. */
    idx_t tmp = n;
    idx_t sum = 0;
    while (tmp > 0) {
        sum += tmp % 10;
        tmp /= 10;
    }

    /* Deficit to reach sum n. */
    idx_t deficit = n - sum;

    /* Minimum number of additional digits needed. */
    idx_t digits_needed = (deficit + 8) / 9;          /* ceil(deficit/9) */

    /* Leading digit of the needed suffix. */
    idx_t first_digit = deficit - 9 * (digits_needed - 1);

    /* Build the suffix value: first_digit followed by (digits_needed-1) nines. */
    seq_t suffix = (seq_t)first_digit;
    for (idx_t i = 1; i < digits_needed; ++i)
        suffix = suffix * 10 + 9;

    /* Compute 10^digits_needed. */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < digits_needed; ++i)
        pow10 = pow10 * 10;

    /* Combine prefix and suffix. */
    seq_t prefix = (seq_t)n;
    return prefix * pow10 + suffix;
}
