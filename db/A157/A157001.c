/*
 * A157001  Fractions x/y, with 1<=x, y<=n, that reduce to (odd)/(even).
 * https://oeis.org/A157001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #pairs (x,y), 1 ≤ x,y ≤ n, with v₂(x) < v₂(y)
 * Range:     u8 n <= 27, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* v₂(k) is the exponent of 2 in the prime factorisation of k */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)            /* for n = 0 or 1 the answer is 0 */
        return 0;

    /* counts[e] = the number of integers k ≤ n with v₂(k) = e   */
    idx_t counts[64];
    size_t e = 0;
    idx_t power = 1;                   /* 2^e */

    while (power <= n) {
        idx_t next_power = power << 1; /* 2^(e+1) */
        if (next_power == 0 || next_power > n) {
            /* next_power has overflowed or exceeds n;
             * use n+1 so that n/next_power == 0                 */
            next_power = n + 1;
        }
        /* count numbers whose 2‑exponent equals e                */
        counts[e] = n / power - n / next_power;
        power = next_power;
        ++e;
    }
    size_t maxE = e;                    /* number of exponent buckets */

    /* Sums over all pairs with smaller exponent first           */
    idx_t result = 0;
    idx_t higher_sum = 0;
    for (e = maxE; e-- > 0; ) {
        result += counts[e] * higher_sum;
        higher_sum += counts[e];
    }

    return (seq_t)result;
}
