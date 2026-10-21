/*
 * A088279  Smallest n-digit palindrome beginning with n.
 * https://oeis.org/A088279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest palindrome of length n+1 that starts with the
 *            decimal representation of n+1 (i.e. digits of n+1, followed by
 *            n+1-2*len(n+1) zeros, followed by reverse(digits(n+1))) a(0)=1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* d = n+1, the starting digits */
    idx_t d = n + 1;

    /* Extract decimal digits of d (least‑significant first) */
    int d_digits[20];                 /* enough for uint64_t */
    int len_d = 0;
    idx_t tmp = d;
    while (tmp > 0) {
        d_digits[len_d++] = tmp % 10;
        tmp /= 10;
    }

    /* Build the palindrome */
    seq_t result = 0;

    /* Append most significant digits of d */
    for (int i = len_d - 1; i >= 0; --i)
        result = result * (seq_t)10 + (seq_t)d_digits[i];

    /* Number of zeros in the middle */
    idx_t zeros = (n + 1) - (idx_t)(2 * len_d);

    for (idx_t i = 0; i < zeros; ++i)
        result *= (seq_t)10;

    /* Append reverse digits of d */
    for (int i = 0; i < len_d; ++i)
        result = result * (seq_t)10 + (seq_t)d_digits[i];

    return result;
}
