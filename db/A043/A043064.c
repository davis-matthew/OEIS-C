/*
 * A043064  a(n) = (s(n)+1)/7, where s(n) = n-th base 7 palindrome that starts
 *          with 6.
 * https://oeis.org/A043064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+1)/7 where s(n) is the n‑th base‑7 palindrome that
 *            starts with the digit 6. The sequence is computed by enumerating
 *            palindromes in increasing decimal value and converting the n-th
 *            one to the required result
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* under‑offset, not used */

    /* ------------------------------------------------------------------
       1) Determine the length L of the n‑th palindrome.
       2) Compute the local index rem (1‑based) within palindromes of that
          length.
       ------------------------------------------------------------------ */
    idx_t rem = n;
    idx_t L = 1;   /* length in base‑7 digits */

    for (;; ++L)
    {
        idx_t freeVars = 0;
        if (L <= 2)
            freeVars = 0;
        else if ((L & 1) == 0)          /* even length */
            freeVars = L / 2 - 1;
        else                           /* odd length */
            freeVars = (L - 1) / 2;

        /* number of palindromes with this length */
        idx_t count = 1;
        for (idx_t i = 0; i < freeVars; ++i)
            count *= 7;

        if (rem <= count)
            break;                      /* found correct length */

        rem -= count;                    /* skip all of this length */
    }

    /* ------------------------------------------------------------------
       3) Build the base‑7 digits of the desired palindrome.
           digits[0] = digits[L-1] = 6.
           The remaining free positions are filled in order
           from the most significant to the least significant
           (i.e. lexicographic order of the numeric value).
       ------------------------------------------------------------------ */
    unsigned char digits[64] = {0};
    digits[0] = 6;
    digits[L - 1] = 6;

    if (L > 2)
    {
        idx_t freeVars = 0;
        if ((L & 1) == 0)
            freeVars = L / 2 - 1;
        else
            freeVars = (L - 1) / 2;

        /* Convert (rem-1) to base‑7, most significant first */
        unsigned char freeDigits[64] = {0};
        idx_t offset = rem - 1;
        for (idx_t i = 0; i < freeVars; ++i)
        {
            /* Write least significant free digit first */
            freeDigits[freeVars - 1 - i] = (unsigned char)(offset % 7);
            offset /= 7;
        }

        /* Place free digits and their mirrors */
        for (idx_t i = 0; i < freeVars; ++i)
        {
            digits[1 + i]          = freeDigits[i];
            digits[L - 2 - i]      = freeDigits[i];
        }
    }

    /* ------------------------------------------------------------------
       4) Convert the digit array from base‑7 to decimal (seq_t).
       ------------------------------------------------------------------ */
    seq_t s = 0;
    for (idx_t i = 0; i < L; ++i)
        s = s * (seq_t)7 + (seq_t)digits[i];

    /* ------------------------------------------------------------------
       5) Return the sequence value.
       ------------------------------------------------------------------ */
    return (s + (seq_t)1) / (seq_t)7;
}
