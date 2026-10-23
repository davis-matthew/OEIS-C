/*
 * A034309  Decimal part of a(n)^(1/3) starts with reversal of its integer
 *          part: first term of runs.
 * https://oeis.org/A034309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Let m = n + 2. Let k be the number of decimal digits of m. Let
 *            rev_m be the decimal reversal of m (leading zeros lost). Then
 *            a(n) = ceil( ( (10^k · m + rev_m)^3 ) / 10^(3k) ). This formula
 *            gives the first term of each run described in A034309
 * Range:     u8 n <= 3, u16 n <= 38, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   pow10, reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/reverse.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /*  m = n + 2 */
    idx_t m = n + 2;

    /* number of decimal digits of m */
    idx_t tmp = m;
    idx_t k   = 0;
    do {
        tmp /= 10;
        k++;
    } while (tmp > 0);

    /* 10^k and reverse(m) */
    seq_t pow10k = pow10((idx_t)k);
    seq_t rev_m  = reverse((seq_t)m);

    /* compute left = 10^k * m + rev_m as unsigned __int128 to avoid overflow */
    unsigned __int128 left128 = (unsigned __int128)pow10k * (unsigned __int128)m
                               + (unsigned __int128)rev_m;

    /* (left)^3 */
    unsigned __int128 num128 = left128 * left128 * left128;

    /* denominator 10^(3k) */
    unsigned __int128 denom128 = 1;
    for (idx_t i = 0; i < 3 * k; i++) {
        denom128 *= 10u;
    }

    /* ceil division */
    unsigned __int128 a128 = (num128 + denom128 - 1) / denom128;

    return (seq_t)a128;
}
