/*
 * A043004  Base-4 palindromes that start with 2.
 * https://oeis.org/A043004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 170, u32 n <= 2100, u64 n <= 2100,
 *            u128 n <= 2100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base-4 palindromes that start with 2
   for the OEIS A043004 sequence */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 1 */

    idx_t remaining = n;   /* how many palindromes we still need to skip */

    for (idx_t len = 1; ; ++len) {
        /* number of digits in the left half (including the fixed 2) */
        idx_t half = (len + 1) / 2;

        /* how many palindromes have this length */
        idx_t cnt = 1;
        for (idx_t i = 1; i < half; ++i)
            cnt *= 4;   /* 4^(half-1) */

        if (remaining <= cnt) {
            /* this length contains the desired palindrome */
            idx_t idx = remaining - 1;  /* zero‑based index within this block */

            /* left half digits: left[0] = most significant (fixed 2) */
            uint8_t left[64];
            left[0] = 2;
            for (idx_t pos = half - 1; pos > 0; --pos) {
                left[pos] = idx & 3;   /* idx % 4 */
                idx >>= 2;            /* idx /= 4 */
            }

            /* build the numeric value from the left half */
            seq_t value = 0;
            for (idx_t i = 0; i < half; ++i)
                value = value * 4 + left[i];

            /* append the mirrored part */
            idx_t start = (len & 1) ? half - 1 : half;
            for (idx_t i = start; i > 0; --i)
                value = value * 4 + left[i - 1];

            return value;
        }

        remaining -= cnt;   /* skip all palindromes of this length */
    }
}
