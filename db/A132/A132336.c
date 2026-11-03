/*
 * A132336  Sum of the integers from 1 to n, excluding perfect fifth powers.
 * https://oeis.org/A132336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 - (2*r^6 + 6*r^5 + 5*r^4 - r^2)/12, where r =
 *            floor(n^(1/5))
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9774 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The calculations below are performed in unsigned __int128 to avoid
   overflow of intermediate results on the 8‑, 16‑, 32‑, 64‑ and 128‑bit
   builds.  The final value is cast to seq_t, which may be an unsigned
   integer or an arbitrary‑precision type, both of which accept conversion
   from a C integer.  */
#include <stdbool.h>

static bool pow5_leq(idx_t base, idx_t limit)
/* Return true iff base^5 <= limit, without overflow. */
{
    unsigned __int128 prod = 1;
    for (int i = 0; i < 5; ++i) {
        if (prod > (unsigned __int128)(limit) / base)
            return false;
        prod *= base;
    }
    return true;
}

static idx_t integer_fifth_root(idx_t n)
/* Return the largest r with r^5 <= n. */
{
    idx_t low = 0;
    idx_t high = 1;

    /* Grow high until high^5 > n.  Stop if high would overflow. */
    while (pow5_leq(high, n)) {
        if (high >= (UINT64_MAX >> 1))
            high = UINT64_MAX;
        else
            high <<= 1;
        if (high == UINT64_MAX && pow5_leq(high, n))
            break;     /* cannot grow any further */
    }

    /* Binary search between low and high (inclusive).  */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);   /* upper mid */
        if (pow5_leq(mid, n))
            low = mid;
        else
            high = mid - 1;
    }

    return low;
}

seq_t a(idx_t n)
{
    idx_t r = integer_fifth_root(n);

    /* Compute the sum of fifth powers up to r using a 128‑bit counter. */
    unsigned __int128 r2 = (unsigned __int128)r * r;
    unsigned __int128 r4 = r2 * r2;
    unsigned __int128 r5 = r4 * r;
    unsigned __int128 r6 = r5 * r;

    unsigned __int128 numerator =
        (unsigned __int128)2 * r6
      + (unsigned __int128)6 * r5
      + (unsigned __int128)5 * r4
      - r2;

    unsigned __int128 sum_powers = numerator / (unsigned __int128)12;

    /* Total sum 1 + 2 + ... + n = n*(n+1)/2 */
    unsigned __int128 total = (unsigned __int128)n * ((unsigned __int128)n + 1) / 2;

    uint64_t diff = (uint64_t)(total - sum_powers);  /* fits in 64 bits */
    return (seq_t)diff;
}
