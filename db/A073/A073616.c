/*
 * A073616  a(n) = Sum_{k=0..n} lpf(binomial(n, k)), where lpf(m) is the least
 *          prime factor of m.
 * https://oeis.org/A073616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{k=0..n} lpf( binomial(n,k) ) (lpf(1)=1)
 * Range:     u8 n <= 46, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute a(n) using prime factor exponents of n! */
seq_t a(idx_t n)
{
    /* By definition a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate lpf array (0 = not yet set) */
    idx_t *lpf = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!lpf) { /* memory allocation failure – return 0 as sentinel */
        return (seq_t)0;
    }
    for (idx_t i = 0; i <= n; ++i) {
        lpf[i] = 0;
    }

    /* Temporary array for exponents of the current prime in i!  */
    uint16_t *vp = (uint16_t *)malloc((n + 1) * sizeof(uint16_t));
    if (!vp) {
        free(lpf);
        return (seq_t)0;
    }

    /* Boolean array for the sieve of Eratosthenes (uint8_t is enough) */
    uint8_t *composite = (uint8_t *)malloc((n + 1) * sizeof(uint8_t));
    if (!composite) {
        free(lpf);
        free(vp);
        return (seq_t)0;
    }
    for (idx_t i = 0; i <= n; ++i) {
        composite[i] = 0;
    }
    if (n >= 0) composite[0] = 1;
    if (n >= 1) composite[1] = 1;

    /* Main loop over primes up to n */
    for (idx_t p = 2; p <= n; ++p) {
        if (composite[p]) {
            continue;            /* not a prime */
        }

        /* Compute vp[i] = exponent of p in i! for all i */
        vp[0] = 0;
        for (idx_t i = 1; i <= n; ++i) {
            uint16_t cnt = 0;
            idx_t t = i;
            while (t % p == 0) {
                ++cnt;
                t /= p;
            }
            vp[i] = vp[i - 1] + cnt;
        }
        uint16_t vp_n = vp[n];

        /* For each k, if this prime divides C(n,k) and lpf not set yet,
           set it to p. */
        for (idx_t k = 0; k <= n; ++k) {
            if (lpf[k] != 0) {
                continue;
            }
            int exp = (int)vp_n - (int)vp[k] - (int)vp[n - k];
            if (exp > 0) {
                lpf[k] = p;   /* first (smallest) prime found */
            }
        }

        /* Mark multiples of p as composite */
        for (idx_t m = p + p; m <= n; m += p) {
            composite[m] = 1;
        }
    }

    /* Set lpf to 1 for coefficients equal to 1 (k=0 or k=n) or
       any remaining 0 entries. */
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t val = (lpf[k] == 0) ? 1 : lpf[k];
        sum += (seq_t)val;
    }

    free(composite);
    free(vp);
    free(lpf);

    return sum;
}
