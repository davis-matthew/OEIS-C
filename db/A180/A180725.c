/*
 * A180725  Smallest power of 20 that begins with n.
 * https://oeis.org/A180725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-01)
 * Helpers:   ipow_idx, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: smallest exponent k such that 20^k starts with decimal n */
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/ipow_idx.h"
#include "../utils/num_digits0.h"

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

seq_t a(idx_t n)
{
    /* number of decimal digits in n */
    idx_t d = num_digits0((uint64_t)n);

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
    return ipow_idx(seq_from_str("20"), k);
}
