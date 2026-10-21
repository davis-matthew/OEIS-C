/*
 * A089183  Palindromes in which, in the first half of the number, digits
 *          appear in the order 1, 0, 2, 3, 1, 0, 2, 3, 1, ...
 * https://oeis.org/A089183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the palindromic number whose first half of digits
 *            follows the repeating pattern 1,0,2,3,1,0,2,3,... For odd n the
 *            middle digit is the next element of the pattern
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are provided by the build system. */

seq_t a(idx_t n)
{
    /* pattern of digits: 1,0,2,3 repeating */
    static const int pattern[4] = {1, 0, 2, 3};
    seq_t value = 0;
    idx_t half = n / 2;          /* number of digits in the left side */

    /* Build the left side of the palindrome */
    for (idx_t i = 0; i < half; ++i) {
        int d = pattern[i & 3];           /* i % 4 using mask */
        value = value * 10 + d;
    }

    /* If n is odd, append the middle digit */
    if (n & 1) {
        int d = pattern[half & 3];
        value = value * 10 + d;
    }

    /* Build the right side by mirroring the left side */
    for (idx_t i = half; i > 0; ) {
        --i;
        int d = pattern[i & 3];
        value = value * 10 + d;
    }

    return value;
}
