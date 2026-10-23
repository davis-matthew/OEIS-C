/*
 * A043046  a(n) = (s(n)+1)/3, where s(n) = n-th base 3 palindrome that starts
 *          with 2.
 * https://oeis.org/A043046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (s(n)+1)/3, where s(n) is the n-th base‑3 palindrome *
 *            starting with 2
 * Range:     u8 n <= 17, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 1.  Find the length L of the n‑th palindrome. */
    idx_t cumulative = 0;   /* number of palindromes of smaller length */
    idx_t L = 1;
    for (;; ++L) {
        idx_t count;
        if (L == 1) {
            count = 1;                 /* only "2" */
        } else {
            /* number of free digits k = ceil((L-2)/2) = (L-1)/2 */
            idx_t k = (L - 1) / 2;
            idx_t cnt = 1;
            for (idx_t i = 0; i < k; ++i) cnt *= (idx_t)3;
            count = cnt;
        }
        if (cumulative + count >= n) break;
        cumulative += count;
    }

    /* 2.  Position of the required palindrome within all of length L
     *     (0‑based). */
    idx_t idx_in_L = n - cumulative - 1;

    /* 3.  Number of free digits. */
    idx_t k = (L - 1) / 2;

    /* 4.  Extract the base‑3 digits of the free part (most significant first). */
    unsigned char free_digits[64] = {0};   /* L will never exceed 64 for the used range */
    for (int pos = (int)k - 1; pos >= 0; --pos) {
        free_digits[pos] = (unsigned char)(idx_in_L % 3);
        idx_in_L /= 3;
    }

    /* 5.  Build the full palindrome in base‑3. */
    unsigned char digits[64] = {0};
    digits[0] = 2;            /* first digit fixed to 2 */
    digits[L - 1] = 2;        /* last digit must also be 2 */
    for (idx_t i = 0; i < k; ++i) {
        digits[i + 1] = free_digits[i];
        digits[L - 2 - i] = digits[i + 1];
    }

    /* 6.  Convert the base‑3 number to decimal (seq_t). */
    seq_t s = 0;
    for (idx_t i = 0; i < L; ++i) {
        s = s * (seq_t)3 + (seq_t)digits[i];
    }

    /* 7.  Compute a(n). */
    return (s + (seq_t)1) / (seq_t)3;
}
