/*
 * A067480  Powers of 2 with initial digit 2.
 * https://oeis.org/A067480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 2^k where k is the n‑th integer for which the leading
 *            decimal digit of 2^k is 2 (i.e. the fractional part of
 *            k·log10(2) lies in [log10(2), log10(3))). This implementation
 *            does not assume any ordering of calls and runs in O(n) time with
 *            small constant factors, suitable for n up to about 10000
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* constants defined with high precision (long double) */
    static const long double LOG10_2 = 0.301029995663981195213738894724L;
    static const long double LOG10_3 = 0.477121254719662439468552134349L;

    /* Start from the first exponent k=1 (2^1 has leading digit 2) */
    idx_t  k   = 1;
    long double f = LOG10_2;            /* fractional part of k·log10(2) */
    idx_t  cnt = 1;                     /* how many terms found so far */

    if (n == 1)
        return pow2(k);                  /* first term is 2 */

    /* iterate over exponents until we reach the n‑th qualifying exponent */
    while (cnt < n)
    {
        ++k;
        f += LOG10_2;
        if (f >= 1.0L)
            f -= 1.0L;                   /* keep only the fractional part */
        if (f >= LOG10_2 && f < LOG10_3)
            ++cnt;                       /* a new term with leading digit 2 found */
    }

    return pow2(k);                      /* 2^k is the desired term */
}
