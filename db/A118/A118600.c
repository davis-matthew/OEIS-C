/*
 * A118600  Palindromes in base 9 (written in base 9).
 * https://oeis.org/A118600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th palindrome in base‑9 (including 0), interpreted
 *            as a decimal integer (the digits are taken as base‑10 digits)
 * Range:     u8 n <= 32, u16 n <= 616, u32 n <= 1457, u64 n <= 1457,
 *            u128 n <= 1457, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: the first palindrome is 0. */
    if (n == (idx_t)1)
        return (seq_t)0;

    /* Remaining index after removing the 0 palindrome. */
    idx_t rem_n = n - (idx_t)1;

    /* Pre‑compute powers of 9 (sufficient for lengths needed). */
    static seq_t pow9[30];
    static int pow9_calc = 0;
    if (!pow9_calc) {
        pow9[0] = (seq_t)1;
        for (int i = 1; i < 30; ++i)
            pow9[i] = pow9[i - 1] * (seq_t)9;
        pow9_calc = 1;
    }

    /* Generate palindromes by increasing length L. */
    for (int L = 1; ; ++L) {
        int halfLen = (L + 1) >> 1;                /* ceil(L/2)          */
        seq_t count  = (seq_t)8 * pow9[halfLen - 1]; /* palindromes of this length */

        if (rem_n <= (idx_t)count) {
            /* Find the palindrome that corresponds to index rem_n. */
            idx_t idx     = rem_n - (idx_t)1;       /* zero–based within this group */
            int  left[30];

            /* Determine leading digit (1…8). */
            left[0] = (int)(( (idx_t)idx / (idx_t)pow9[halfLen - 1] ) + (seq_t)1);

            /* Remaining part of the index handles the remaining half‑digits. */
            idx_t rest = idx % (idx_t)pow9[halfLen - 1];
            for (int i = 1; i < halfLen; ++i) {
                idx_t factor = (idx_t)pow9[halfLen - 1 - i];
                left[i] = (int)(rest / factor % (seq_t)9);
                rest    = rest % factor;
            }

            /* Build the full list of digits (mirror the left side). */
            int digits[30];
            for (int i = 0; i < L; ++i)
                digits[i] = (i < halfLen) ? left[i] : left[L - 1 - i];

            /* Interpret the digits as a decimal integer. */
            seq_t val = (seq_t)0;
            for (int i = 0; i < L; ++i)
                val = val * (seq_t)10 + (seq_t)digits[i];

            return val;
        }

        /* Skip this block of palindromes and continue with larger lengths. */
        rem_n -= (idx_t)count;
    }
}
