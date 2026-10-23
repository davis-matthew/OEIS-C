/*
 * A132286  Odd palindromes with an even number of digits divided by 11.
 * https://oeis.org/A132286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (odd palindrome with an even number of digits of index n)
 *            / 11
 * Range:     u8 n <= 5, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the nth term of A132286.  n is 1-indexed. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* behaviour for n < 1 is irrelevant */

    idx_t       k        = 1;            /* block index (length = 2k)   */
    seq_t       pow10_k   = 10;           /* 10^k                        */
    seq_t       pow10_km1 = 1;            /* 10^(k-1)                     */
    seq_t       cum_k     = 5 * (pow10_k - 1) / 9;  /* cumulative count up to k   */

    /* Find the block that contains the nth palindrome. */
    while ((seq_t)n > cum_k) {
        ++k;
        pow10_km1 = pow10_k;
        pow10_k   = pow10_k * 10;
        cum_k     = 5 * (pow10_k - 1) / 9;
    }

    /* Cumulative count up to the previous block. */
    seq_t prev_cum;
    if (k > 1) {
        prev_cum = 5 * (pow10_km1 - 1) / 9;
    } else {
        prev_cum = 0;
    }

    /* Offset of the desired palindrome within this block (0‑based). */
    seq_t offset = (seq_t)n - prev_cum - 1;
    seq_t hsize  = pow10_km1;               /* 10^(k-1) */

    /* Determine the first digit and the remaining part of the left half. */
    idx_t group      = (idx_t)(offset / hsize);    /* 0 .. 4          */
    seq_t rem        = offset % hsize;
    seq_t first_digit = 1 + (seq_t)(2 * group);    /* odd digit 1,3,5,7,9 */

    /* Build the k‑digit left half of the palindrome. */
    seq_t half = first_digit * hsize + rem;

    /* Reverse the left half to form the right half. */
    seq_t rev = 0;
    seq_t tmp = half;
    while (tmp > 0) {
        rev = rev * 10 + (tmp % 10);
        tmp /= 10;
    }

    /* Construct the full even‑length palindrome. */
    seq_t palindrome = half * pow10_k + rev;

    /* Return the value divided by 11. */
    return palindrome / 11;
}
