/*
 * A043005  Base-4 palindromes that start with 3.
 * https://oeis.org/A043005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth base‑4 palindrome that starts with digit 3,
 *            enumerated in increasing order
 * Range:     u8 n <= 10, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th base‑4 palindrome starting with 3.  The
 * function works for any unsigned integer type seq_t provided
 * by the build and for any idx_t (uint64_t). */
seq_t a(idx_t n)
{
    if (n == 0) /* Undefined for offset 0; return 0 for safety */
        return (seq_t)0;

    /* Find the length L of the palindrome and offset within that length. */
    idx_t L = 1;
    seq_t rem = (seq_t)(n - 1); /* 0‑based index */

    while (1)
    {
        idx_t k = (L + 1) / 2;   /* number of independent digits */
        seq_t combos = 1;
        for (idx_t i = 0; i < k - 1; ++i)
            combos *= 4;         /* 4^(k-1) */

        if (rem < combos)
            break;
        rem -= combos;
        ++L;
    }

    /* Decode offset into the middle digits (positions 1 … k-1). */
    idx_t k = (L + 1) / 2;
    uint8_t digits[64] = {0};
    digits[0] = 3;              /* leading digit fixed to 3 */
    idx_t offset = (idx_t)rem;   /* offset fits in idx_t (see loop above) */

    for (int pos = (int)(k - 2); pos >= 0; --pos)
    {
        digits[pos + 1] = offset % 4;
        offset /= 4;
    }

    /* Mirror to obtain the full palindrome. */
    if ((L & 1) == 0)          /* even length */
    {
        for (idx_t j = 0; j < k; ++j)
            digits[L - 1 - j] = digits[j];
    }
    else                       /* odd length */
    {
        for (idx_t j = 0; j < k - 1; ++j)
            digits[L - 1 - j] = digits[j];
    }

    /* Convert base‑4 digits to a decimal value. */
    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i)
        val = val * 4 + digits[i];

    return val;
}
