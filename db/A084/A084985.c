/*
 * A084985  Palindromes in sequence A084984.
 * https://oeis.org/A084985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth palindrome whose decimal representation uses only the digits
   0,1,4,6,8,9 (in increasing order). */
seq_t a(idx_t n) {
    static const uint8_t digits_nonzero[5] = {1,4,6,8,9};
    static const uint8_t digits_all[6]    = {0,1,4,6,8,9};
    /* powers of 6 up to 6^10 (more than enough for all test cases) */
    static const idx_t pow6[11] = {1,6,36,216,1296,7776,46656,279936,
                                   1679616,10077696,60466176};

    idx_t remain = n;
    idx_t len = 1;

    /* Step 1: find the length of the nth palindrome */
    while (1) {
        idx_t count;
        if (len == 1 || len == 2) {
            count = 5;
        } else if (len % 2 == 1) {          /* odd length */
            int m = (len - 1) / 2;
            count = 5 * pow6[m];
        } else {                            /* even length */
            int m = len / 2;
            count = 5 * pow6[m - 1];
        }
        if (remain <= count)
            break;
        remain -= count;
        ++len;
    }

    idx_t k = remain - 1;                    /* 0‑based index within this length */
    int leftLen = (len + 1) / 2;             /* digits that determine the palindrome */
    uint8_t left[20] = {0};

    if (len == 1) {
        left[0] = digits_nonzero[k];
    } else if (len == 2) {
        left[0] = digits_nonzero[k];
    } else if (len % 2 == 1) {              /* odd length */
        int m = (len - 1) / 2;
        idx_t firstIndex = k / pow6[m];
        idx_t rem = k % pow6[m];
        left[0] = digits_nonzero[firstIndex];
        for (int i = 1; i <= m; ++i) {
            idx_t idx = rem / pow6[m - i] % 6;
            left[i] = digits_all[idx];
        }
    } else {                                 /* even length */
        int m = len / 2;
        idx_t firstIndex = k / pow6[m - 1];
        idx_t rem = k % pow6[m - 1];
        left[0] = digits_nonzero[firstIndex];
        for (int i = 1; i < m; ++i) {
            idx_t idx = rem / pow6[m - 1 - i] % 6;
            left[i] = digits_all[idx];
        }
    }

    /* Build the full digit array of the palindrome */
    uint8_t digits[20] = {0};
    int i;
    for (i = 0; i < leftLen; ++i)
        digits[i] = left[i];
    for (i = 0; i < len / 2; ++i)
        digits[len - 1 - i] = digits[i];

    /* Convert digits to seq_t value */
    seq_t result = 0;
    for (i = 0; i < len; ++i)
        result = result * 10 + (seq_t)digits[i];

    return result;
}
