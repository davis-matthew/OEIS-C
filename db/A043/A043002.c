/*
 * A043002  Base-3 palindromes that start with 2.
 * https://oeis.org/A043002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th base-3 palindrome that starts with 2
 * Range:     u8 n <= 17, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is 1‑based, but handle 0 gracefully */

    /* Find the length L of the palindrome that contains the n-th term. */
    idx_t cumulative = 0;   /* total count of palindromes with length < current L */
    idx_t L = 0;            /* current length being examined */

    for (L = 1;; ++L) {
        /* Count of palindromes of this length:
           for length L = 1,2,3,... the count is 3^floor((L-1)/2).  */
        idx_t k      = (L + 1) / 2;      /* ceil(L/2) */
        idx_t lenVar = k - 1;            /* number of free digits (excluding the first) */
        idx_t count  = 1;
        for (idx_t i = 0; i < lenVar; ++i) count *= 3;   /* 3^lenVar */

        if (cumulative + count >= n) break;   /* n-th term lies in this length */
        cumulative += count;
    }

    /* zero‑based offset within palindromes of length L */
    idx_t offset = n - cumulative - 1;

    /* Build the digit sequence of the palindrome. */
    uint8_t digits[64];  /* depth is limited (≤ 18 for n≈10 000) */
    digits[0] = 2;       /* first digit is fixed to 2 */

    idx_t k      = (L + 1) / 2;      /* ceil(L/2) */
    idx_t lenVar = k - 1;            /* free digits */

    /* Determine the value of each free digit from the offset (base‑3). */
    for (idx_t j = 0; j < lenVar; ++j) {
        idx_t divisor = 1;
        for (idx_t p = 0; p < lenVar - 1 - j; ++p) divisor *= 3;
        digits[j + 1] = (uint8_t)((offset / divisor) % 3);
    }

    /* The last digit must also be 2. */
    digits[L - 1] = 2;

    /* Mirror the free digits to the right side of the palindrome. */
    for (idx_t i = 1; i < k; ++i) {
        idx_t dst = L - 1 - i;
        if (dst == k - 1) continue;  /* skip the center when L is odd */
        digits[dst] = digits[i];
    }

    /* Convert the base‑3 digit array to the decimal value. */
    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i) {
        val = val * 3 + digits[i];
    }
    return val;
}
