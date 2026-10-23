/*
 * A043263  Sum of the digits of the n-th base 4 palindrome.
 * https://oeis.org/A043263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   sum of the digits of the n-th base‑4 palindrome
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t idx = n - 1;                 /* zero‑based index, a(1) has index 0 */

    /* first palindromes: 0, 1, 2, 3 (length 1) */
    if (idx < 4)
        return (seq_t)idx;

    idx -= 4;                           /* skip the 1‑digit palindromes */
    idx_t L = 2;                        /* candidate length */

    /* Find the length L of the n‑th palindrome */
    while (1)
    {
        idx_t k, pow = 1, count;
        if (L & 1)                     /* odd length: L = 2k+1 */
        {
            k = (L - 1) / 2;
            for (idx_t i = 0; i < k; ++i)
                pow *= 4;              /* pow = 4^k               */
            count = 3 * pow;           /* 3 choices for first digit */
        }
        else                           /* even length: L = 2k     */
        {
            k = L / 2;
            for (idx_t i = 0; i < k - 1; ++i)
                pow *= 4;              /* pow = 4^(k-1)           */
            count = 3 * pow;
        }

        if (idx < count)
            break;
        idx -= count;
        ++L;
    }

    /* Compute the digit sum of the palindrome of length L */
    seq_t sum = 0;

    if (L & 1)                         /* odd length */
    {
        idx_t k = (L - 1) / 2;
        idx_t pow = 1;
        for (idx_t i = 0; i < k; ++i)
            pow *= 4;                  /* pow = 4^k               */

        idx_t first_idx = idx / pow;
        seq_t first_digit = (seq_t)(1 + first_idx);
        idx_t rem = idx % pow;

        seq_t middle_digit = (seq_t)(rem % 4);
        rem /= 4;                      /* remove the middle digit   */

        seq_t sum_first = first_digit;
        for (idx_t i = 0; i < k - 1; ++i)
        {
            seq_t d = (seq_t)(rem % 4);
            rem /= 4;
            sum_first += d;
        }
        sum = 2 * sum_first + middle_digit;   /* full digit sum          */
    }
    else                                 /* even length */
    {
        idx_t k = L / 2;
        idx_t pow = 1;
        for (idx_t i = 0; i < k - 1; ++i)
            pow *= 4;                  /* pow = 4^(k-1)           */

        idx_t first_idx = idx / pow;
        seq_t first_digit = (seq_t)(1 + first_idx);
        idx_t rem = idx % pow;

        seq_t sum_first = first_digit;
        for (idx_t i = 0; i < k - 1; ++i)
        {
            seq_t d = (seq_t)(rem % 4);
            rem /= 4;
            sum_first += d;
        }
        sum = 2 * sum_first;                      /* mirror halves            */
    }

    return sum;
}
