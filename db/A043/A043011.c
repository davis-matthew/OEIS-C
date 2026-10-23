/*
 * A043011  Base-6 palindromes that start with 2.
 * https://oeis.org/A043011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth base‑6 palindrome whose leading digit is 2
 * Range:     u8 n <= 8, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A043011: base‑6 palindromes that start with 2.
 * The sequence is defined for n ≥ 1.  The implementation constructs the
 * palindrome of length `len` that contains the `offset`‑th free digit
 * combination in lexicographic order, where `offset` runs from 0.
 * Complexity: O(L) per call, where L is the number of base‑6 digits
 * (L ≤ 11 for n ≤ 10000).  Operations are performed in the generic
 * integer type seq_t; the algorithm uses only integer arithmetic. */
seq_t a(idx_t n)
{
    /* Find the length of the palindrome containing the nth element. */
    idx_t len = 1;                /* current length candidate          */
    idx_t remaining = n;          /* how many terms still needed      */
    while (1) {
        idx_t m = (len - 1) / 2;  /* number of free base‑6 digits     */
        /* count = 6^m, guaranteed to be < 10 000 for len ≤ 11. */
        idx_t count = 1;
        for (idx_t k = 0; k < m; ++k) count *= 6;
        if (remaining <= count) break;
        remaining -= count;
        ++len;
    }

    /* remaining is 1‑based position within the length‑`len` block. */
    idx_t m = (len - 1) / 2;
    idx_t offset = remaining - 1;        /* 0‑based index of free digits   */

    /* Convert offset into base‑6 digits (most significant first). */
    seq_t free_digits[20];               /* m ≤ 5 for n ≤ 10000           */
    for (idx_t i = 0; i < m; ++i) {
        free_digits[m - 1 - i] = offset % 6;
        offset /= 6;
    }

    /* Build the full palindrome digit array. */
    seq_t digits[20] = {0};
    digits[0] = 2;                       /* leading digit fixed to 2       */
    for (idx_t j = 1; j < len; ++j) {
        if (j <= m)                        /* free digit positions          */
            digits[j] = free_digits[j - 1];
        else                               /* mirrored positions            */
            digits[j] = digits[len - 1 - j];
    }

    /* Convert digit array to integer value. */
    seq_t value = 0;
    for (idx_t j = 0; j < len; ++j) {
        value = value * 6 + digits[j];
    }
    return value;
}
