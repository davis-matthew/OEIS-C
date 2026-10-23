/*
 * A029951  Even palindromes.
 * https://oeis.org/A029951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth even decimal palindrome (base 10) The sequence is
 *            0, 2, 4, 6, 8, 22, 44, 66, 88, 202, …
 * Range:     u8 n <= 15, u16 n <= 344, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static idx_t pow10_uint(idx_t e) {
    idx_t r = 1;
    while (e--) r *= 10;
    return r;
}

static const seq_t odd_digits[5] = { (seq_t)0, (seq_t)2, (seq_t)4, (seq_t)6, (seq_t)8 };
static const seq_t even_lead[4] = { (seq_t)2, (seq_t)4, (seq_t)6, (seq_t)8 };

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* safety: offset starts at 1 */

    /* length 1: 0,2,4,6,8 */
    if (n <= 5) return odd_digits[n - 1];

    /* remaining index after first 5 terms */
    idx_t remaining = n - 5;
    idx_t len = 2;                     /* start with length 2 */

    /* find the correct length */
    for (;;) {
        idx_t k = (len + 1) / 2;            /* number of free digits in the first half */
        idx_t groupSize = pow10_uint(k - 1); /* 10^(k-1) */
        idx_t count_len = 4 * groupSize;    /* 4 choices for the leading even digit */

        if (remaining <= count_len) break;
        remaining -= count_len;
        len++;
    }

    idx_t k = (len + 1) / 2;
    idx_t groupSize = pow10_uint(k - 1);
    idx_t pos = remaining - 1;                     /* 0‑based position inside this length */
    idx_t lead_index = pos / groupSize;            /* which leading even digit */
    seq_t lead = even_lead[lead_index];

    idx_t rem = pos % groupSize;                   /* remainder for the rest of the digits */

    /* build the first half of the palindrome */
    seq_t firstHalf = lead;
    for (idx_t i = 0; i < k - 1; ++i) {
        idx_t power = pow10_uint(k - 2 - i);
        idx_t digit = rem / power;
        rem %= power;
        firstHalf = firstHalf * 10 + (seq_t)digit;
    }

    /* construct the full palindrome by mirroring */
    seq_t result = firstHalf;
    seq_t temp   = firstHalf;
    if (len % 2 == 1) temp /= 10;           /* skip middle digit for odd lengths */
    while (temp > 0) {
        result = result * 10 + (seq_t)(temp % 10);
        temp /= 10;
    }

    return result;
}
