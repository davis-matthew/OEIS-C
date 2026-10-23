/*
 * A045574  Numbers that are still numbers when turned upside down (uses only
 *          digits 0, 1, 6, 8, 9 with no final 0's).
 * https://oeis.org/A045574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1201, u32 n <= 9405, u64 n <= 9405,
 *            u128 n <= 9405, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A045574: numbers that are still numbers when turned upside down
   (digits 0,1,6,8,9 with no final 0), listed in increasing order. */
seq_t a(idx_t n)
{
    /* Special case for the single zero. */
    if (n == 1) {
        return (seq_t)0;
    }

    /* Zero-based rank among the numbers that exclude the initial 0. */
    idx_t k = n - 2;

    /* Length information: len is the number of digits of the current block. */
    idx_t len        = 1;    /* start counting from numbers of length 1 (excluding 0) */
    idx_t powmid     = 1;    /* 5^(len-2); unused for len==1 */
    idx_t count_len  = 4;    /* counts for len==1 (digits 1,6,8,9) */
    idx_t cumulative = 0;    /* total numbers before the current block */

    /* Advance to the block that contains the k‑th number. */
    while (cumulative + count_len <= k) {
        cumulative += count_len;
        ++len;
        if (len == 1) {            /* never taken after the first iteration */
            count_len = 4;
            powmid    = 1;
        } else if (len == 2) {
            count_len = 16;         /* 4 * 4 */
            powmid    = 1;
        } else {
            powmid *= 5;            /* 5^(len-2) */
            count_len = 16 * powmid;
        }
    }

    idx_t offset = k - cumulative;   /* position inside the block */

    /* Digit tables. */
    static const int firstDigits[4]  = {1, 6, 8, 9};
    static const int middleDigits[5] = {0, 1, 6, 8, 9};
    static const int lastDigits[4]   = {1, 6, 8, 9};

    /* Build the number. */
    seq_t result = 0;

    if (len == 1) {
        /* Four single-digit numbers: 1,6,8,9. */
        result = (seq_t)firstDigits[(int)offset];
        return result;
    }

    /* Decompose the offset into [first][middle][last]. */
    idx_t factor       = powmid * 4;            /* combinations for a fixed first digit */
    idx_t first_idx    = offset / factor;
    idx_t rem          = offset % factor;
    idx_t middle_index = rem / 4;
    idx_t last_idx     = rem % 4;

    result = (seq_t)firstDigits[(int)first_idx];

    /* Convert middle_index into base‑5 digits (most significant first). */
    int middle_count = (int)(len - 2);
    int mid_digits[64];            /* large enough for all n expected by the tests */
    idx_t tmp = middle_index;
    for (int i = middle_count - 1; i >= 0; --i) {
        mid_digits[i] = (int)(tmp % 5);
        tmp /= 5;
    }
    for (int i = 0; i < middle_count; ++i) {
        result = result * (seq_t)10 + (seq_t)middleDigits[mid_digits[i]];
    }

    /* Append the last digit. */
    result = result * (seq_t)10 + (seq_t)lastDigits[(int)last_idx];

    return result;
}
