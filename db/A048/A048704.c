/*
 * A048704  Base 4 palindromes of even length divided by 5. a(n) =
 *          A048703(n)/5.
 * https://oeis.org/A048704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th base‑4 palindrome of even length divided by 5
 * Range:     u8 n <= 15, u16 n <= 297, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Extract base‑4 digits of n (least significant first). */
    idx_t temp = n;
    int digs[64];
    int m = 0;
    while (temp > 0 && m < 64) {
        digs[m++] = temp % 4;
        temp /= 4;
    }

    /* Build the even‑length palindrome in base 4 and interpret
       it as a base‑10 number. */
    seq_t val = 0;

    /* Append the left half (most significant to least). */
    for (int i = m - 1; i >= 0; --i)
        val = val * 4 + (seq_t)digs[i];

    /* Append the mirrored right half. */
    for (int i = 0; i < m; ++i)
        val = val * 4 + (seq_t)digs[i];

    /* All such palindromes are multiples of 5. */
    return val / 5;
}
