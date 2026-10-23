/*
 * A029950  Odd palindromes.
 * https://oeis.org/A029950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer palindrome whose last digit is odd
 *            (i.e. the first digit is odd). The sequence lists all such
 *            palindromes in increasing order, with a(1) = 1. The
 *            implementation enumerates the palindromes by length, computing
 *            the appropriate first half of the palindrome and mirroring it
 * Range:     u8 n <= 20, u16 n <= 410, u32 n <= 7110, u64 n <= 7110,
 *            u128 n <= 7110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for the offset */

    /* pre‑compute powers of 10 in idx_t to avoid overflow for counting */
    static idx_t pow10[128];
    static int pow10_init = 0;
    if (!pow10_init) {
        pow10_init = 1;
        pow10[0] = (idx_t)1;
        for (int i = 1; i < 128; ++i)
            pow10[i] = pow10[i - 1] * (idx_t)10;
    }

    idx_t idx = n - 1;                  /* 0‑based index into the sequence */
    int L = 1;                          /* current palindrome length */

    /* find the length that contains the n‑th palindrome */
    while (1) {
        int k = (L + 1) / 2;            /* digits in the first half */
        idx_t block = pow10[k - 1];     /* 10^(k-1) */
        idx_t count = (idx_t)5 * block; /* 5 choices for the first digit */
        if (idx < count)
            break;
        idx -= count;
        ++L;
    }

    int k = (L + 1) / 2;
    idx_t block = pow10[k - 1];
    idx_t first_idx = idx / block;      /* 0..4 */
    int first_digit = (int)((first_idx * 2) + 1); /* map 0..4 → 1,3,5,7,9 */
    idx_t rem = idx % block;            /* remaining positions */

    /* digit array: digits[0] is the first digit, digits[1..k-1] the rest */
    int digits[128];
    digits[0] = first_digit;
    for (int i = k - 1; i >= 1; --i) {
        digits[i] = (int)(rem % 10);
        rem /= 10;
    }

    /* build the left half of the palindrome */
    seq_t result = (seq_t)0;
    for (int i = 0; i < k; ++i)
        result = result * (seq_t)10 + (seq_t)digits[i];

    /* mirror the left half to form the full palindrome */
    int mirror_start = (L % 2 == 0) ? (k - 1) : (k - 2);
    for (int i = mirror_start; i >= 0; --i)
        result = result * (seq_t)10 + (seq_t)digits[i];

    return result;
}
