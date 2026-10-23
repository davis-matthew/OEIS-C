/*
 * A180725  Smallest power of 20 that begins with n.
 * https://oeis.org/A180725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   digit_count, seq_from_str, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: smallest exponent k such that 20^k starts with decimal n */
#include <math.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_from_str.h"
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    /* number of decimal digits in n */
    idx_t d = digit_count((uint64_t)n);

    /* pre‑compute interval for the fractional part of k*log10(2) */
    long double nld = (long double)n;
    long double dld = (long double)d;
    long double low  = log10l(nld) - (dld - 1.0L);
    long double high = log10l(nld + 1.0L) - (dld - 1.0L);

    const long double LOG10_2 = log10l(2.0L);
    idx_t k = 0;

    for (;; ++k)
    {
        long double f = fmodl((long double)k * LOG10_2, 1.0L);
        if (f >= low && f < high)
            break;
    }

    /* 20^k = 2^k * 10^k, computed directly via seq_pow */
    return seq_pow(seq_from_str("20"), k);
}
