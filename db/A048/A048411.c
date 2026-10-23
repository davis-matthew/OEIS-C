/*
 * A048411  Squares whose consecutive digits differ by 1.
 * https://oeis.org/A048411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Squares whose consecutive digits differ by 1
   a(n) = (R_{n-4})^2          for n >= 7
   R_k = (10^k - 1)/9          repunit of length k
   Special cases: a(1)=0, a(2)=1, a(3)=4, a(4)=9, a(5)=121, a(6)=676
*/

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first six terms explicitly */
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 4;
    if (n == 4) return 9;
    if (n == 5) return 121;
    if (n == 6) return 676;

    /* For n >= 7: compute square of a repunit of length L = n-4 */
    idx_t L = n - 4;                    /* length of repunit (>=3) */
    size_t len = (size_t)(2 * L - 1);   /* number of digit positions before carry */

    /* Allocate buffer for least‑significant to most‑significant digits */
    uint8_t *digits = (uint8_t *)malloc(len + 20); /* tiny over‑allocation for possible carry */
    if (!digits) return 0;              /* allocation failure → return 0 as a safe fallback */

    size_t pos = 0;
    uint64_t carry = 0;

    /* Build the raw coefficient array and apply carry immediately */
    for (idx_t i = 0; i < (idx_t)len; ++i) {
        uint64_t coeff = (i < L) ? (i + 1) : (2 * L - 1 - i);
        uint64_t sum   = coeff + carry;
        digits[pos++] = (uint8_t)(sum % 10);
        carry = sum / 10;
    }
    while (carry) {
        digits[pos++] = carry % 10;
        carry /= 10;
    }

    /* Convert the digit array (least‑significant first) into seq_t */
    seq_t result = 0;
    for (long long i = (long long)pos - 1; i >= 0; --i) {
        result = result * 10 + (seq_t)digits[i];
    }

    free(digits);
    return result;
}
