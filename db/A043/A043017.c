/*
 * A043017  Base-7 palindromes that start with 3.
 * https://oeis.org/A043017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: Generates the n-th base‑7 palindrome whose most
 * significant digit is 3.  The sequence is defined by all numbers that
 * read the same forwards and backwards in base 7 and begin with the
 * digit 3.  This implementation works for any seq_t and idx_t
 * supplied by the build.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* offset is 1 */

    /* Find the length L of the palindrome that contains the n‑th
       number.  For each length L there are 7^(ceil(L/2)-1) such palindromes. */
    idx_t        L          = 1;
    seq_t        cumulative = 0;
    seq_t        count      = 0;

    for (;; ++L) {
        idx_t  k = (L + 1) / 2;               /* number of free digits in the left half */
        seq_t  pow = 1;
        for (idx_t e = 0; e < k-1; ++e)        /* pow = 7^(k-1) */
            pow *= (seq_t)7;
        count = pow;
        cumulative += count;
        if (cumulative >= (seq_t)n)
            break;
    }

    /* Zero‑based index within the block of length L palindromes */
    seq_t cumulative_before = cumulative - count;
    idx_t offset = (idx_t)((seq_t)n - cumulative_before - 1);

    /* Build the digits of the palindrome in base 7. */
    int      digits[64] = {0};                 /* support very large lengths */
    idx_t    k          = (L + 1) / 2;

    digits[0] = 3;                              /* most significant digit fixed to 3 */
    idx_t    tmp = offset;
    for (idx_t i = k-1; i > 0; --i) {           /* remaining free digits of the left half */
        digits[i] = (int)(tmp % 7);
        tmp      /= 7;
    }

    for (idx_t i = k; i < L; ++i)               /* mirror to the right half */
        digits[i] = digits[L-1-i];

    /* Convert the base‑7 digits to a decimal value. */
    seq_t value = 0;
    for (idx_t i = 0; i < L; ++i)
        value = value * (seq_t)7 + (seq_t)digits[i];

    return value;
}
