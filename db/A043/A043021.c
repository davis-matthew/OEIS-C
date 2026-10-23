/*
 * A043021  Base-8 palindromes that start with 1.
 * https://oeis.org/A043021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 146, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Base‑8 palindromes that start with 1 – enumerated in increasing numeric order.
   For index n (1‑based) this function returns the n-th such palindrome. */
#include <stdbool.h>

/* pre‑compute powers of 8 up to a reasonable limit (enough for any practical length) */
static seq_t pow8[64];
static bool pow8_inited = false;

static void init_pow8(void)
{
    pow8[0] = (seq_t)1;
    for (int i = 1; i < 64; ++i)
        pow8[i] = pow8[i - 1] * (seq_t)8;
    pow8_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined for offset 0, safety */

    if (!pow8_inited)
        init_pow8();

    const seq_t sn = (seq_t)n;          /* index as seq_t for comparisons */
    seq_t cum = 0;                      /* cumulative count of palindromes up to previous length */
    idx_t len = 1;                      /* current palindrome length */

    /* Find the length that contains the n-th palindrome */
    for (;; ++len)
    {
        seq_t cnt;
        if (len == 1)
            cnt = (seq_t)1;
        else
        {
            idx_t half = (len + 1) / 2;          /* number of independent digits */
            idx_t free = half - 1;               /* digits after the leading 1 */
            cnt = pow8[free];
        }

        if (cum + cnt >= sn)
            break;
        cum += cnt;
    }

    if (len == 1)                          /* trivial case */
        return (seq_t)1;

    /* Position within palindromes of this length (0‑based) */
    seq_t offset = sn - cum - 1;

    idx_t half = (len + 1) / 2;             /* number of independent digits */
    seq_t digits[64];                       /* store independent digits */
    digits[0] = (seq_t)1;                    /* leading digit fixed to 1 */

    seq_t tmp = offset;
    for (idx_t i = half - 1; i >= 1; --i)   /* fill the remaining digits */
    {
        digits[i] = tmp % 8;
        tmp /= 8;
    }

    /* Compose the full base‑8 palindrome and convert to decimal */
    seq_t val = 0;
    for (idx_t pos = 0; pos < len; ++pos)
    {
        seq_t d = (pos < half) ? digits[pos] : digits[len - 1 - pos];
        val = val * 8 + d;
    }

    return val;
}
