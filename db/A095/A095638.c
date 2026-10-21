/*
 * A095638  a(n) = 124 written in base n.
 * https://oeis.org/A095638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = representation of the decimal number 124 in base n,
 *            written as a single decimal integer obtained by concatenating
 *            the base‑n digits. For n = 1 the value is written in unary (124
 *            copies of the digit 1). The function works for any n ≥ 1 and
 *            uses only integer arithmetic on seq_t
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute 10^k as a seq_t, where k ≤ 3. */
static seq_t pow10_int(int k)
{
    seq_t r = 1;
    for (int i = 0; i < k; ++i) r *= 10;
    return r;
}

/* Number of decimal digits of a non‑negative integer x (0 ≤ x ≤ 999). */
static int dec_digits(int x)
{
    if (x < 10) return 1;
    if (x < 100) return 2;
    return 3;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1)    /* unary representation – 124 copies of 1 */
    {
        seq_t res = 0;
        for (int i = 0; i < 124; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* 124 in decimal.  For n ≥ 2, convert 124 to base n. */
    const unsigned long long VAL = 124ULL;
    unsigned long long temp = VAL;
    int digits[64];            /* sufficient for any n up to 2^64 */
    int cnt = 0;

    /* Gather base-n digits (least significant first). */
    while (temp > 0)
    {
        digits[cnt++] = (int)(temp % n);
        temp /= n;
    }

    /* Build the concatenated decimal integer. */
    seq_t res = 0;
    for (int i = cnt - 1; i >= 0; --i)
    {
        int d = digits[i];
        res = res * pow10_int(dec_digits(d)) + d;
    }
    return res;
}
