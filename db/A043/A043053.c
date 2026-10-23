/*
 * A043053  a(n) = (s(n) + 1)/5, where s(n) = n-th base-5 palindrome that
 *          starts with 4.
 * https://oeis.org/A043053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n) + 1)/5 where s(n) is the n‑th base‑5 palindrome
 *            that starts with 4. The function constructs the palindrome
 *            directly and then returns (s + 1)/5
 * Range:     u8 n <= 7, u16 n <= 197, u32 n <= 937, u64 n <= 937,
 *            u128 n <= 937, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const idx_t POW5[10] = {
    1ULL, 5ULL, 25ULL, 125ULL, 625ULL,
    3125ULL, 15625ULL, 78125ULL, 390625ULL, 1953125ULL
};

seq_t a(idx_t n)
{
    /* Convert n to a zero‑based index within the list of palindromes. */
    idx_t remaining = n - 1;          /* 0‑based offset */
    idx_t length = 1;                /* current palindrome length */

    /* Find the length that contains the n‑th palindrome. */
    while (1) {
        idx_t h = (length + 1) >> 1;          /* ceil(length/2) */
        idx_t e = h - 1;                      /* number of free digits */
        idx_t cnt = POW5[e];                  /* how many palindromes of this length */

        if (remaining < cnt) break;           /* n-th lies here */
        remaining -= cnt;                     /* skip whole block */
        ++length;
    }

    /* Build the palindrome digits (most significant first). */
    int digits[20] = {0};                      /* max length needed is 13 */
    digits[0] = 4;
    digits[length - 1] = 4;

    if (length > 1) {
        idx_t h = (length + 1) >> 1;
        idx_t k = remaining;                   /* zero‑based offset within this length */

        /* Fill the middle h-1 digits from the offset k (base‑5). */
        for (int i = (int)(h - 1) - 1; i >= 0; --i) {
            digits[i + 1] = k % 5;
            k /= 5;
        }

        /* Mirror the first half to complete the palindrome. */
        for (int i = 0; i < h; ++i) {
            digits[length - 1 - i] = digits[i];
        }
    }

    /* Convert the base‑5 digit array to a decimal value. */
    seq_t value = 0;
    for (idx_t i = 0; i < length; ++i) {
        value = value * 5 + (seq_t)digits[i];
    }

    /* Return (value + 1)/5 as required. */
    return (value + 1) / 5;
}
