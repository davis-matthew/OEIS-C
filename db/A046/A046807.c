/*
 * A046807  Palindromes in factorial base.
 * https://oeis.org/A046807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n-th smallest integer whose factorial base
 *            representation is a palindrome. The sequence begins 0, 1, 3, 7,
 *            9, 11, 25, 33, 41, 121,
 * Range:     u8 n <= 21, u16 n <= 153, u32 n <= 5913, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;          /* first term is 0 */

    idx_t pos = n - 1;            /* rank among the positive palindromes */

    /* Find the factorial-base length that contains the desired term */
    idx_t len = 1;
    while (1) {
        idx_t count;
        if (len == 1) {           /* only the value 1 for length 1 */
            count = 1;
        } else {
            idx_t m = len / 2;
            /* product of (i+1) for i = 2..m */
            idx_t prod = 1;
            for (idx_t i = 2; i <= m; ++i)
                prod *= (i + 1);
            if (len & 1)          /* odd length: multiply by (m+2) */
                prod *= (m + 2);
            count = prod;
        }
        if (pos <= count)
            break;
        pos -= count;
        ++len;
    }

    if (len == 1)                /* only one positive number of length 1 */
        return (seq_t)1;

    /* Precompute factorials 1! … len! */
    seq_t *fact = (seq_t *)malloc((len + 1) * sizeof(seq_t));
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= len; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Precompute suffix products of the number of possibilities
       for the remaining variable digits. */
    idx_t m = len / 2;
    idx_t *prod_from = (idx_t *)malloc((m + 2) * sizeof(idx_t));
    prod_from[m + 1] = (idx_t)1;           /* no remaining variables */
    for (idx_t i = m; i >= 2; --i)
        prod_from[i] = prod_from[i + 1] * (i + 1);

    idx_t mid_factor = (len & 1) ? (m + 2) : (idx_t)1;   /* (mid+1) if odd */

    /* Allocate array for the digits of the palindrome */
    seq_t *digit = (seq_t *)malloc((len + 1) * sizeof(seq_t));
    digit[1] = (seq_t)1;              /* d1 must be 1 */
    digit[len] = (seq_t)1;            /* dk must be 1 */

    /* Unrank the palindrome digits */
    for (idx_t i = 2; i <= m; ++i) {
        for (idx_t d = 0; d <= i; ++d) {
            idx_t block = prod_from[i + 1] * mid_factor;
            if (pos > block)
                pos -= block;
            else {
                digit[i] = (seq_t)d;
                digit[len - i + 1] = digit[i];
                break;
            }
        }
    }

    if (len & 1) {                    /* middle digit for odd length */
        idx_t mid = m + 1;
        for (idx_t d = 0; d <= mid; ++d) {
            if (pos > 1)
                pos -= 1;
            else {
                digit[mid] = (seq_t)d;
                break;
            }
        }
    }

    /* Compute the value from the digits and factorials */
    seq_t value = (seq_t)0;
    for (idx_t i = 1; i <= len; ++i)
        value += digit[i] * fact[i];

    /* Free allocated memory */
    free(fact);
    free(prod_from);
    free(digit);

    return value;
}
