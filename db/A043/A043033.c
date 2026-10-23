/*
 * A043033  Base-9 palindromes that start with 6.
 * https://oeis.org/A043033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth base‑9 palindrome whose first digit is 6 (decimal
 *            value)
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; n ≥ 1 */
    idx_t cumulative = 0;   /* count of terms before current length */
    idx_t L = 0;            /* current length of the palindromes */
    idx_t k = 0;            /* number of free digits in this length */

    /* Find the length L such that the desired index lies in its block */
    while (1) {
        ++L;
        k = (L + 1) / 2 - 1;          /* ceil(L/2) - 1  */
        idx_t pow9 = 1;
        for (idx_t i = 0; i < k; ++i)
            pow9 *= 9;                /* 9^k  */
        if (cumulative + pow9 >= n)
            break;
        cumulative += pow9;
    }

    idx_t offset = n - cumulative - 1;          /* 0‑based offset within this length */
    k = (L + 1) / 2 - 1;

    /* Convert offset to base‑9 digits (most‑significant first) */
    unsigned char digits[64];   /* k ≤ 20 for all n that fit in a 64‑bit index */
    idx_t m = offset;
    for (idx_t i = 0; i < k; ++i) {
        digits[k - 1 - i] = m % 9;
        m /= 9;
    }

    /* Build the full digit array of the palindrome */
    unsigned char arr[64];
    arr[0] = 6;
    arr[L - 1] = 6;
    for (idx_t i = 0; i < k; ++i) {
        arr[1 + i] = digits[i];
        arr[L - 2 - i] = digits[i];
    }

    /* Convert the base‑9 digit array to a base‑10 value of type seq_t */
    seq_t value = 0;
    for (idx_t i = 0; i < L; ++i)
        value = value * 9 + arr[i];

    return value;
}
