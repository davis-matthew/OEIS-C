/*
 * A043007  Base-5 palindromes that start with 2.
 * https://oeis.org/A043007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043007: Base‑5 palindromes that start with 2. */
/* The n-th term is the n-th smallest decimal value of a base‑5
   palindrome whose most significant digit is 2.  The function
   works for any unsigned n ≥ 1, for any size of seq_t. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;       /* undefined for n < 1 */

    /* Find the length L of the palindrome that contains
       the n‑th term and the offset within that block. */
    idx_t cum = 0;          // number of terms before current length
    idx_t L   = 1;          // current length, starts at 1

    while (1) {
        idx_t count;        // number of palindromes of length L

        if (L == 1) {
            count = 1;
        } else if ((L & 1) == 0) {        /* even: L = 2*k */
            idx_t k = L >> 1;             /* k = L/2            */
            count = 1;
            for (idx_t i = 0; i < k - 1; ++i) count *= 5;
        } else {                          /* odd:  L = 2*k + 1 */
            idx_t k = (L - 1) >> 1;
            count = 1;
            for (idx_t i = 0; i < k; ++i) count *= 5;
        }

        if (cum + count >= n) break;      /* target length found */
        cum += count;
        ++L;
    }

    idx_t off = n - cum - 1;              // 0‑based offset in this block

    /* Length 1 is only the single digit 2. */
    if (L == 1) return (seq_t)2;

    /* Number of free (non‑fixed) digits in the first half. */
    idx_t freeDigits = (L & 1) ? ((L - 1) >> 1) : ((L >> 1) - 1);

    /* Convert the offset to base‑5 digits for the free positions.
       digits[0] holds the most significant free digit. */
    uint8_t digits[64] = {0};
    idx_t tmp = off;
    for (idx_t i = 0; i < freeDigits; ++i) {
        digits[freeDigits - 1 - i] = (uint8_t)(tmp % 5);
        tmp /= 5;
    }

    /* Build the decimal value from the base‑5 digits. */
    seq_t value = 2;   /* leading 2 */

    if ((L & 1) == 0) {                     /* even length: 2k */
        for (idx_t i = 0; i < freeDigits; ++i)
            value = value * 5 + (seq_t)digits[i];
        /* mirror the free digits */
        for (idx_t i = 0; i < freeDigits; ++i)
            value = value * 5 + (seq_t)digits[freeDigits - 1 - i];
    } else {                                 /* odd length: 2k+1 */
        for (idx_t i = 0; i < freeDigits - 1; ++i)
            value = value * 5 + (seq_t)digits[i];
        /* middle digit */
        value = value * 5 + (seq_t)digits[freeDigits - 1];
        /* mirror the first free digits */
        for (idx_t i = 0; i < freeDigits - 1; ++i)
            value = value * 5 + (seq_t)digits[freeDigits - 2 - i];
    }

    /* trailing 2 */
    value = value * 5 + (seq_t)2;

    return value;
}
