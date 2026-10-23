/*
 * A176923  Squares of A057148 taken as decimal numbers.
 * https://oeis.org/A176923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (p(n))^2 * where p(n) is the nth decimal palindrome
 *            composed only of digits 0 and 1, * sorted by value, with p(1)=0,
 *            p(2)=1, …
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;

    /* index among non‑zero palindromes (p(2)=1 is the first) */
    idx_t m = n - (idx_t)1;
    idx_t total = 0;
    idx_t L = 0;

    while (1) {
        ++L;
        idx_t cnt;
        if (L == 1) {
            cnt = 1;                     /* only the value 1 */
        } else if ((L & 1) == 0) {       /* even length 2k */
            idx_t k = L >> 1;
            cnt = ((idx_t)1) << (k - 1); /* 2^(k-1) */
        } else {                          /* odd length 2k+1 */
            idx_t k = (L - 1) >> 1;
            cnt = ((idx_t)1) << k;        /* 2^k */
        }

        if (m <= total + cnt)
            break;
        total += cnt;
    }

    idx_t offset = m - total - (idx_t)1;   /* 0‑based within the group of length L */

    /* Build the decimal digits of the palindrome */
    int digits[64];
    size_t pos = 0;

    if (L == 1) {
        digits[pos++] = 1;
    } else if ((L & 1) == 0) {      /* even length L = 2k */
        idx_t k = L >> 1;
        digits[pos++] = 1;                     /* leading digit */

        idx_t var_bits = k - 1;                 /* bits for the remaining left side */
        for (idx_t i = 0; i < var_bits; ++i) {
            int bit = (int)((offset >> (var_bits - 1 - i)) & 1);
            digits[pos++] = bit;
        }

        /* mirror the left part to the right */
        for (idx_t i = 0; i < k; ++i)
            digits[pos++] = digits[k - 1 - i];
    } else {                     /* odd length L = 2k+1 */
        idx_t k = (L - 1) >> 1;                    /* number of bits after the leading 1 */
        digits[pos++] = 1;                         /* leading digit */

        for (idx_t i = 1; i <= k; ++i) {           /* bits for positions 2 .. k+1 (includes middle) */
            int bit = (int)((offset >> (k - i)) & 1);
            digits[pos++] = bit;
        }

        /* mirror the left part excluding the middle digit */
        for (idx_t i = 0; i < k; ++i)
            digits[pos++] = digits[k - 1 - i];
    }

    /* Convert digits to a seq_t value */
    seq_t pal = 0;
    for (idx_t i = 0; i < pos; ++i)
        pal = pal * (seq_t)10 + (seq_t)digits[i];

    return pal * pal;
}
