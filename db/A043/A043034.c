/*
 * A043034  Base-9 palindromes that start with 7.
 * https://oeis.org/A043034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate base‑9 palindromes that start with 7 */
seq_t a(idx_t n)
{
    /* iterate over increasing digit lengths */
    for (idx_t L = 1;; ++L) {
        idx_t half = (L + 1) / 2;
        idx_t exp  = half - 1;

        /* number of palindromes of this length */
        idx_t count = 1;
        for (idx_t i = 0; i < exp; ++i) count *= 9u;

        if (n <= count) {                 /* first palindrome of length L we need */
            idx_t m = n - 1;               /* 0‑based index within this length */

            uint8_t digits[32] = {0};      /* enough for all L we use (≤~15) */
            digits[0] = 7;                 /* leading digit fixed to 7 */

            /* fill the remaining digits of the left half */
            for (int pos = (int)half - 1; pos >= 1; --pos) {
                digits[pos] = m % 9u;
                m /= 9u;
            }

            /* mirror the left half to the right half */
            for (idx_t i = half; i < L; ++i) {
                digits[i] = digits[L - 1 - i];
            }

            /* convert base‑9 digits to decimal */
            seq_t val = 0;
            for (idx_t i = 0; i < L; ++i) {
                val = val * 9u + digits[i];
            }
            return val;
        }

        /* move to next length */
        n -= count;
    }
}
