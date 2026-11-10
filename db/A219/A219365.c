/*
 * A219365  a(n) = P(n)/(L(n)*P(n/2)*P(n/3)*P(n/7)*P(n/43)*...) with P(n) =
 *          floor(n)!, L(n) the LCM of the first n integers and where the
 *          sequence 2, 3, 7, 43, ... is A000058.
 * https://oeis.org/A219365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!/(lcm(1..n) * Product_{k>=0} floor(n/A000058(k))!),
 *            computed prime by prime via Legendre's formula
 * Range:     u8 n <= 57, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A219365_leg(uint64_t m, uint64_t p)
{
    uint64_t s = 0;
    while (m) { m /= p; s += m; }
    return s;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    if (n < 2) return result;

    /* Sylvester terms <= n */
    uint64_t syl[80];
    int ns = 0;
    {
        unsigned __int128 s = 2;
        while (s <= (unsigned __int128)n && ns < 80) {
            syl[ns++] = (uint64_t)s;
            s = s * (s - 1) + 1;
        }
    }

    size_t sz = (size_t)n + 1;
    unsigned char *comp = (unsigned char *)calloc(sz, 1);
    if (!comp) return result;

    for (uint64_t p = 2; p <= n; p++) {
        if (comp[p]) continue;
        if (p <= n / p)
            for (uint64_t j = p * p; j <= n; j += p) comp[j] = 1;

        int64_t e = (int64_t)A219365_leg(n, p);
        for (int i = 0; i < ns; i++)
            e -= (int64_t)A219365_leg(n / syl[i], p);
        /* lcm exponent: floor(log_p n) */
        {
            uint64_t t = n;
            while (t >= p) { t /= p; e--; }
        }
        for (int64_t i = 0; i < e; i++)
            result *= (seq_t)p;
    }
    free(comp);
    return result;
}
