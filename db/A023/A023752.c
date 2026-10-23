/*
 * A023752  Plaindromes: numbers whose digits in base 11 are in nondecreasing
 *          order.
 * https://oeis.org/A023752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=0; for n>=2, let r = n‑2 (zero‑based index among positive
 *            numbers). Find the smallest length L such that sum_{i=1}^{L}
 *            C(i+9,i) > r . Then choose the r‑th weakly increasing sequence
 *            of length L over the digits 1…10 (lexicographically by the
 *            digits) and interpret it as a base‑11 number
 * Range:     u8 n <= 121, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* compute binomial coefficient C(a,b) using seq_t arithmetic. */
static seq_t binom(idx_t a, idx_t b)
{
    if (b > a) return 0;
    if (b > a - b) b = a - b;
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i)
        res = res * (seq_t)(a - b + i) / (seq_t)i;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;          /* a(1) = 0 */

    idx_t idx = n - 2;              /* zero‑based rank among positive numbers */

    /* Determine the length (number of base‑11 digits). */
    idx_t len = 1;
    while (1)
    {
        seq_t cnt_big = binom((idx_t)len + 9U, len);  /* count of length‑len numbers */
        idx_t cnt = (idx_t)cnt_big;
        if (idx < cnt)
            break;
        idx -= cnt;
        ++len;
    }

    /* Unrank the number within the obtained length. */
    idx_t rank = idx;              /* 0‑based rank */
    unsigned char digits[64] = {0};

    for (idx_t pos = 0; pos < len; ++pos)
    {
        unsigned char start = (pos == 0) ? 1 : digits[pos - 1];
        for (unsigned char d = start; d <= 10; ++d)
        {
            idx_t rem = len - pos - 1;
            seq_t cnt_big = binom((idx_t)(10 - d + rem), rem);
            idx_t cnt = (idx_t)cnt_big;
            if (rank < cnt)
            {
                digits[pos] = d;
                break;
            }
            else
            {
                rank -= cnt;
            }
        }
    }

    /* Convert the digit array back to a base‑10 number. */
    seq_t result = 0;
    for (idx_t i = 0; i < len; ++i)
        result = result * (seq_t)11 + (seq_t)digits[i];

    return result;
}
