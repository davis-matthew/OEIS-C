/*
 * A029955  Palindromic in base 9.
 * https://oeis.org/A029955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 818, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer whose base‑9 representation is a palindrome. */
seq_t a(idx_t n)
{
    idx_t cum = 0;
    idx_t L = 1;
    /* find the length L of the nth palindrome */
    while (1) {
        idx_t cnt;
        if (L == 1) {
            cnt = 9;
        } else {
            idx_t r = (L + 1) / 2;
            idx_t pow9 = 1;
            for (idx_t i = 0; i < r - 1; ++i)
                pow9 *= 9;
            cnt = 8 * pow9;
        }
        if (cum + cnt >= n) break;
        cum += cnt;
        ++L;
    }

    idx_t k = n - cum - 1;           /* zero‑based index within length L */
    idx_t r = (L + 1) / 2;

    idx_t pow9_half = 1;
    for (idx_t i = 0; i < r - 1; ++i)
        pow9_half *= 9;

    seq_t first_half;
    if (L == 1)
        first_half = k;
    else
        first_half = (seq_t)(pow9_half + k);

    int digits_first[32];
    seq_t tmp = first_half;
    for (idx_t i = 0; i < r; ++i) {
        digits_first[r - 1 - i] = (int)(tmp % 9);
        tmp /= 9;
    }

    int digits[32];
    for (idx_t i = 0; i < r; ++i)
        digits[i] = digits_first[i];

    idx_t offset = (L % 2 == 0) ? r : r - 1;
    for (idx_t i = 0; i < offset; ++i)
        digits[r + i] = digits_first[offset - 1 - i];

    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i)
        val = val * 9 + (seq_t)digits[i];

    return val;
}
