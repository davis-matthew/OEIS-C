/*
 * A132285  Odd palindromes with an even number of digits.
 * https://oeis.org/A132285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 55, u32 n <= 555, u64 n <= 555,
 *            u128 n <= 555, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the n-th odd palindrome having an even number of digits */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    idx_t cum = 0;               /* cumulative count of terms processed so far */
    idx_t pow10i = 1;            /* 10^(k-1) as an idx_t (integer)          */
    seq_t pow10s = 1;            /* 10^(k-1) as a seq_t (may be wider)     */
    for (idx_t k = 1; ; ++k) {
        idx_t count = 5 * pow10i;                 /* 5⋅10^(k‑1) palindromes of length 2k */
        if (n <= cum + count) {
            idx_t pos = n - cum - 1;              /* zero‑based index inside the block  */
            idx_t digit_idx = pos / pow10i;       /* which odd first digit (0→1,1→3…)  */
            idx_t rem       = pos % pow10i;       /* remaining lower digits             */

            seq_t first_digit = 1 + 2 * digit_idx;    /* 1,3,5,7,9                          */
            seq_t H = first_digit * pow10s + (seq_t)rem; /* first half of the palindrome */

            seq_t pow10 = pow10s * 10;                /* 10^k                              */
            /* compute reverse(H) */
            seq_t revH = 0;
            seq_t tmp = H;
            while (tmp > 0) {
                revH = revH * 10 + (tmp % 10);
                tmp /= 10;
            }
            return H * pow10 + revH;
        }
        cum += count;
        pow10i *= 10;
        pow10s *= 10;
    }
}
