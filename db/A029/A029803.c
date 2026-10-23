/*
 * A029803  Numbers that are palindromic in base 8.
 * https://oeis.org/A029803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth number whose base‑8 representation is a palindrome
 *            (a(1)=0)
 * Range:     u8 n <= 39, u16 n <= 639, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first term directly */
    if (n == 1) return 0;

    /* Zero‑based index of the desired palindrome */
    idx_t idx = n - 1;          /* idx in [0, …] */

    /* Find the length L of the base‑8 palindrome that contains idx */
    idx_t L = 1;
    idx_t cumulative = 0;       /* count of palindromes with length < L */

    while (1) {
        /* Count of palindromes of length L */
        idx_t count;
        if (L == 1) {
            count = 8;
        } else {
            idx_t h = (L + 1) / 2;      /* ceil(L/2) */
            count = 7;                 /* first digit non‑zero */
            for (idx_t i = 1; i < h; ++i) {
                count *= 8;            /* remaining digits 0–7 */
            }
        }

        if (idx < cumulative + count) break;  /* idx lies within this length */
        cumulative += count;
        ++L;
    }

    /* offset of the desired palindrome within the block of length L */
    idx_t offset = idx - cumulative;

    /* Compute the first half value (most significant digits) */
    idx_t h = (L + 1) / 2;                      /* ceil(L/2) */
    seq_t first_half;
    if (L == 1) {
        first_half = (seq_t)offset;             /* single digit: 0–7 */
    } else {
        seq_t min_first = 1;                    /* smallest first digit is 1 */
        for (idx_t i = 1; i < h; ++i) min_first *= (seq_t)8;
        first_half = min_first + (seq_t)offset;
    }

    /* Extract the base‑8 digits of the first half (most significant first) */
    seq_t digits[32];        /* enough for the maximum L that we need */
    seq_t tmp = first_half;
    for (idx_t i = 0; i < h; ++i) {
        digits[h - 1 - i] = tmp % (seq_t)8;
        tmp /= (seq_t)8;
    }

    /* Build the complete palindrome digit by digit, accumulating in seq_t */
    seq_t result = 0;
    if (L % 2 == 0) {
        /* even length: mirror the entire first half */
        for (idx_t i = 0; i < h; ++i) {
            result = result * (seq_t)8 + digits[i];
        }
        for (idx_t i = 0; i < h; ++i) {
            result = result * (seq_t)8 + digits[h - 1 - i];
        }
    } else {
        /* odd length: mirror all digits except the middle one */
        for (idx_t i = 0; i < h - 1; ++i) {
            result = result * (seq_t)8 + digits[i];
        }
        result = result * (seq_t)8 + digits[h - 1];          /* middle digit */
        for (idx_t i = 0; i < h - 1; ++i) {
            result = result * (seq_t)8 + digits[h - 2 - i];
        }
    }

    return result;
}
