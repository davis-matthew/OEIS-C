/*
 * A118597  Palindromes in base 6 (written in base 6).
 * https://oeis.org/A118597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 251, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = the n‑th palindrome in base 6 using only digits 0–5, with no leading zero
   (except the single digit 0), written as a decimal integer.
   The first few terms are  
   n=1..6 : 0 1 2 3 4 5
   n=7..11: 11 22 33 44 55
   n=12..  : 101 111 121… 
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first 6 single‑digit palindromes directly */
    if (n <= 6)
        return (seq_t)(n - 1);

    /* Remaining index after the first 6 numbers */
    idx_t rem = n - 6;

    /* Start inspecting palindromes of increasing length */
    idx_t len = 2;          /* current palindrome length under consideration */

    while (1) {
        /* k = number of independent digits (first half rounded up) */
        idx_t k = (len + 1) / 2;

        /* Compute 6^(k-1) safely in idx_t */
        idx_t pow = 1;
        for (idx_t i = 0; i < k - 1; ++i)
            pow *= 6;

        /* Count of palindromes of this length */
        idx_t count = 5 * pow;   /* first digit 1–5, remaining k-1 digits 0–5 */

        if (rem <= count)       /* this length contains the desired palindrome */
            break;

        rem -= count;           /* skip all palindromes of this length          */
        ++len;                  /* move to next length */
    }

    /* rem is the 1‑based index within palindromes of length 'len' */
    idx_t idx0 = rem - 1;  /* convert to 0‑based index */
    idx_t k = (len + 1) / 2;

    /* Compute 6^(k-1) again for digit extraction */
    idx_t pow = 1;
    for (idx_t i = 0; i < k - 1; ++i)
        pow *= 6;

    /* Determine the first independent digit (1–5) */
    idx_t first_index = idx0 / pow;          /* 0‑based: 0→1, 1→2, …, 4→5 */
    idx_t rem_digits = idx0 % pow;           /* remaining part for the rest of the digits */

    /* Build the independent digits array */
    uint8_t ind[20] = {0};          /* length never exceeds ~10 for n <= 10 000 */
    ind[0] = (uint8_t)(1 + first_index);
    for (idx_t pos = k - 1; pos; --pos) {    /* positions k-1 down to 1 */
        ind[pos] = (uint8_t)(rem_digits % 6);
        rem_digits /= 6;
    }

    /* Assemble the full palindrome digits (mirror the first k digits) */
    uint8_t digits[20] = {0};
    for (idx_t i = 0; i < k; ++i)
        digits[i] = ind[i];
    for (idx_t i = k; i < len; ++i)
        digits[i] = digits[len - 1 - i];

    /* Convert the digit sequence to a decimal integer stored in seq_t */
    seq_t value = 0;
    for (idx_t i = 0; i < len; ++i)
        value = value * 10 + (seq_t)digits[i];

    return value;
}
