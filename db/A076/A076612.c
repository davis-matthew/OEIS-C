/*
 * A076612  Palindromic numbers with nonprime middle digit.
 * https://oeis.org/A076612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the n-th odd‑length decimal palindrome whose middle
 *            digit is not prime (digits 0,1,4,6,8,9). The sequence is
 *            produced by counting the number of palindromes of each odd
 *            length and constructing the palindrome that corresponds to the
 *            requested index
 * Range:     u8 n <= 14, u16 n <= 392, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* nonprime digits allowed in the middle position */
static const int MID_DIGITS[6] = {0, 1, 4, 6, 8, 9};
/* one‑digit palindromes (all odd‑length palindromes of length 1) */
static const int SINGLE_DIGITS[5] = {1, 4, 6, 8, 9};

/* Index and value types are supplied by the build.	*/
seq_t a(idx_t n)
{
    /* 1‑based index.  n == 0 is undefined – we return 0 as a guard. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Find the odd length 2k‑1 that contains the n‑th palindrome. */
    idx_t cumulative = 0;	/* terms before the current length */
    int k = 1;		/* current k so length = 2*k‑1 */

    while (1) {
        idx_t count_k;
        if (k == 1) {
            /* length 1: only the single‑digit palindromes */
            count_k = 5;
        } else {
            /* For k ≥ 2:
               • the first digit (most significant) can be 1–9 (9 choices)
               • the remaining k‑2 digits (before the middle) are free 0–9
               • the middle digit has 6 allowed choices
               Count = 9 * 10^(k‑2) * 6			*/
            idx_t pow10 = 1;
            for (int i = 0; i < k - 2; ++i) pow10 *= 10ULL;
            count_k = 9ULL * pow10 * 6ULL;
        }

        if (n <= cumulative + count_k) {
            break;	/* the desired palindrome is in this length */
        }
        cumulative += count_k;
        ++k;
    }

    /* 0‑based index within palindromes of this length */
    idx_t idx_in_len = n - cumulative - 1;

    /* Length 1: simply return the corresponding digit */
    if (k == 1) {
        return (seq_t)SINGLE_DIGITS[(int)idx_in_len];
    }

    /* For longer lengths split the index into left part and middle digit. */
    idx_t left_index = idx_in_len / 6;		/* 0‑based index of the left part */
    int mid_index   = (int)(idx_in_len % 6);	/* 0‑based index of the middle digit */

    /* Compute the numeric value of the left part:
       * base_left = 10^(k‑2)  (the smallest left part when the first digit is 1)
       * left_value = base_left + left_index
    */
    seq_t base_left = 1;
    for (int i = 0; i < k - 2; ++i) base_left *= 10U;
    seq_t left_value = base_left + (seq_t)left_index;

    /* Extract the left‑half digits (most significant first). */
    int left_len   = k - 1;			/* number of digits on one side */
    int left_digits[10];				/* left_len ≤ 4 for n ≤ 10 000 */

    seq_t divisor = base_left;		/* 10^(k‑2) */
    seq_t temp_left = left_value;
    for (int pos = 0; pos < left_len; ++pos) {
        left_digits[pos] = (int)((temp_left / divisor) % 10U);
        temp_left %= divisor;
        if (divisor > 1U) divisor /= 10U;
    }

    /* Assemble the palindrome: left part, middle digit, reverse of left part. */
    seq_t result = 0;

    for (int pos = 0; pos < left_len; ++pos) {
        result = result * 10U + (seq_t)left_digits[pos];
    }

    result = result * 10U + (seq_t)MID_DIGITS[mid_index];

    for (int pos = left_len - 1; pos >= 0; --pos) {
        result = result * 10U + (seq_t)left_digits[pos];
    }

    return result;
}
