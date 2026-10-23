/*
 * A121041  Number of divisors of n that are also contained in the decimal
 *          representation of n.
 * https://oeis.org/A121041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of divisors of n that appear as decimal *
 *            substrings of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

/* Integer square root of an unsigned 64‑bit integer.
 * Return floor(sqrt(n)).  Uses binary search with overflow protection. */
static idx_t int_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid > 0 && mid > n / mid) {       /* mid*mid would overflow or exceed n */
            hi = mid - 1;
        } else {
            idx_t sq = mid * mid;
            if (sq == n) return mid;
            if (sq < n)
                lo = mid + 1;
            else
                hi = mid - 1;
        }
    }
    return hi; /* hi is floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1; safe guard */

    /* decimal representation of n */
    char nstr[32];
    snprintf(nstr, sizeof nstr, "%" PRIu64, (uint64_t)n);

    seq_t count = (seq_t)0;
    idx_t root = int_sqrt_u64(n);

    char dstr[32];
    for (idx_t i = 1; i <= root; ++i) {
        if (n % i == 0) {
            /* divisor i */
            snprintf(dstr, sizeof dstr, "%" PRIu64, (uint64_t)i);
            if (strstr(nstr, dstr) != NULL)
                count += (seq_t)1;

            /* corresponding divisor n/i */
            idx_t d2 = n / i;
            if (d2 != i) {
                snprintf(dstr, sizeof dstr, "%" PRIu64, (uint64_t)d2);
                if (strstr(nstr, dstr) != NULL)
                    count += (seq_t)1;
            }
        }
    }

    return count;
}
