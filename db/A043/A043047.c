/*
 * A043047  a(n) = (s(n)+3)/4, where s(n) is the n-th base-4 palindrome that
 *          starts with 1 (A043003).
 * https://oeis.org/A043047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+3)/4, where s(n) is the n-th base‑4 palindrome
 *            starting with 1
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; values for n==0 are undefined but return 0 safely */
    if (n == 0) return 0;

    idx_t cumulative = 0;

    for (idx_t L = 1; ; ++L) {
        /* half = ceil(L/2) */
        idx_t half = (L + 1) / 2;

        /* number of palindromes of this length with leading digit 1 */
        idx_t cnt = 1;
        for (idx_t i = 0; i < half - 1; ++i) {
            cnt *= 4;
        }

        if (cumulative + cnt < n) {
            cumulative += cnt;
            continue;
        }

        /* n falls in length L */
        idx_t k   = n - cumulative;        /* 1‑based index within block L */
        idx_t tail = k - 1;                 /* 0‑based variable part */
        idx_t lenTail = half - 1;           /* number of flexible digits */

        uint8_t first[64];
        first[0] = 1;                       /* leading digit fixed */

        /* Fill the remaining digits of the first half from most to least
         * significant. The least significant base‑4 digit of 'tail' goes
         * into the rightmost position of the first half. */
        if (lenTail > 0) {
            for (idx_t i = 0; i < lenTail; ++i) {
                first[half - 1 - i] = tail % 4;
                tail /= 4;
            }
        }

        /* Build the full palindrome value in base 4 */
        seq_t val = 0;
        for (idx_t i = 0; i < half; ++i) {
            val = val * 4 + first[i];
        }

        if (L > 1) {
            idx_t start = (L & 1) ? half - 2 : half - 1;
            for (idx_t i = start + 1; i > 0; --i) {
                val = val * 4 + first[i - 1];
            }
        }

        /* final transformation */
        return (val + 3) / 4;
    }
}
