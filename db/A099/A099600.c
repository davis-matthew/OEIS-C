/*
 * A099600  Inventory application: Numerator of the probability (rounded to
 *          the nearest trillionth: (10^-12)-th) of at least one shortage per
 *          month when n customers each request one item on one
 *          independently-chosen random day of a 30-day month, but when only
 *          exactly one such item is available per day.
 * https://oeis.org/A099600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = round(10^12 * (1 - 30! / ((30-n)! * 30^n))) for 0 <= n <=
 *            30 * a(n) = 10^12 for n > 30 * * This implementation evaluates
 *            the fraction step‑by‑step, cancelling * common factors in every
 *            step to keep the numerator and denominator * small. The final
 *            result is computed with 128‑bit integers to...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b != (unsigned __int128)0)
    {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* The probability is 0 for n=0, and 1 (i.e. 10^12) for n>30 */
    if (n > 30)
        return (seq_t)1000000000000ULL;

    unsigned __int128 num = 1;   /* numerator of the product of (30-k)/30 */
    unsigned __int128 den = 1;   /* denominator of the product of (30-k)/30 */

    for (idx_t k = 0; k < n; ++k)
    {
        uint64_t pk = 30 - k;          /* (30-k) */
        /* cancel common factors between pk and current denominator */
        unsigned __int128 g1 = gcd128((unsigned __int128)pk, den);
        num *= (unsigned __int128)(pk / g1);
        den /= g1;

        /* cancel common factors between current numerator and 30 */
        unsigned __int128 g2 = gcd128(num, (unsigned __int128)30);
        den *= (unsigned __int128)(30 / g2);
        num /= g2;
    }

    /* at this point num/den = probability of no collision */
    unsigned __int128 delta = den - num;                 /* den - num >= 0 */
    unsigned __int128 product = delta * (unsigned __int128)1000000000000ULL;

    unsigned __int128 q = product / den;
    unsigned __int128 r = product % den;

    unsigned __int128 res = q;
    if ((r << 1) >= den) /* round to nearest integer, half up */
        ++res;

    return (seq_t)res;
}
