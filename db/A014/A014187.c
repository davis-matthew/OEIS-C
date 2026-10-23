/*
 * A014187  Cubes of palindromes.
 * https://oeis.org/A014187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (p(n))^3, where p(n) is the nth decimal palindrome
 *            (starting with 0)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool type */

static idx_t pow10_idx(idx_t e)
{
    idx_t r = 1;
    while (e--) r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    /* Handle the first 10 trivial palindromes (0‑9). */
    if (n < 10) {
        seq_t x = (seq_t)n;
        return x * x * x;
    }

    idx_t pos = n - 10;           /* position within current length group */
    idx_t len = 2;                /* current palindrome length */
    idx_t count_len = 9;          /* number of palindromes of the current length */

    /* Find the length of the palindrome that contains index n. */
    while (pos >= count_len) {
        pos -= count_len;
        ++len;
        if (len % 2 == 0) {
            count_len = 9 * pow10_idx(len / 2 - 1);
        } else {
            count_len = 9 * pow10_idx((len - 1) / 2);
        }
    }

    /* Determine the leading half of the palindrome. */
    idx_t halfLen;
    if (len % 2 == 0) {
        halfLen = len / 2;
    } else {
        halfLen = (len + 1) / 2;
    }
    idx_t base = pow10_idx(halfLen - 1);
    seq_t half = (seq_t)base + (seq_t)pos;

    /* Construct the full palindrome. */
    seq_t pal = half;
    seq_t temp = half;
    if (len % 2 == 1) temp /= 10;  /* skip the middle digit for odd lengths */
    while (temp > 0) {
        pal = pal * 10 + (temp % 10);
        temp /= 10;
    }

    /* Return the cube of the palindrome. */
    return pal * pal * pal;
}
