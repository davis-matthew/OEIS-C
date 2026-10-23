/*
 * A043003  Base-4 palindromes that start with 1.
 * https://oeis.org/A043003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th base-4 palindrome with most significant digit 1
 * Range:     u8 n <= 10, u16 n <= 170, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n == 0 (outside offset) gracefully */
    if (n == 0U)
        return 0;

    /* Find the length L of the palindrome for the given index n */
    unsigned __int128 cum = 0;          /* cumulative count of palindromes */
    unsigned __int128 count_len = 0;    /* count for the current length */
    int len = 0;
    int exp = 0;                        /* exponent of 4: exp = ceil(L/2) - 1 */

    while (cum < n) {
        len++;
        int half_ceil = (len + 1) / 2;
        exp = half_ceil - 1;

        /* Compute count_len = 4^exp using 128-bit arithmetic */
        count_len = 1;
        for (int i = 0; i < exp; ++i)
            count_len *= 4U;

        cum += count_len;
    }

    /* cumulative count of palindromes of all shorter lengths */
    unsigned __int128 cum_prev = cum - count_len;
    /* index of the required number within the current length (0‑based) */
    idx_t idx_in_len = n - (idx_t)cum_prev - 1U;

    /* Prepare the left half digits (including the leading 1) */
    int half_ceil = (len + 1) / 2;          /* number of independent digits */
    unsigned char digits[64];               /* sufficient for expected lengths */
    digits[0] = 1;                          /* leading digit is always 1 */

    for (int i = exp - 1; i >= 0; --i) {    /* fill remaining digits in MSB order */
        digits[1 + i] = (unsigned char)(idx_in_len % 4U);
        idx_in_len /= 4U;
    }

    /* Compute the decimal value by traversing the full palindrome */
    seq_t value = 0;
    for (int i = 0; i < len; ++i) {
        unsigned char d;
        if (i < half_ceil)
            d = digits[i];
        else
            d = digits[len - 1 - i];
        value = value * 4 + d;
    }

    return value;
}
