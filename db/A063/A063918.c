/*
 * A063918  a(1) = 1 and - applying the sieve of Eratosthenes - for n > 1:
 *          a(n) = if n is prime then 0 else the first prime p which marks n
 *          as composite.
 * https://oeis.org/A063918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n==1; 0 if n is prime; otherwise smallest prime
 *            factor of n. Implemented with a pre‑computed least‑prime‑factor
 *            table (sieve)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    static seq_t *spf = NULL;
    static const idx_t LIMIT = 200000;   /* enough for n up to about 10 000 */
    static bool initialized = false;

    if (!initialized) {
        /* allocate array of size LIMIT+1, indices 0..LIMIT */
        spf = (seq_t *)malloc((LIMIT + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= LIMIT; ++i) spf[i] = 0;

        for (idx_t i = 2; i <= LIMIT; ++i) {
            if (spf[i] == 0) {                  /* i is prime */
                /* start marking from i*i to avoid labeling the prime itself */
                if (i > LIMIT / i) continue;   /* prevent overflow of i*i */
                for (idx_t j = i * i; j <= LIMIT; j += i) {
                    if (spf[j] == 0) spf[j] = i;
                }
            }
        }
        initialized = true;
    }

    if (n <= LIMIT) {
        /* spf[k]==0 for primes, non‑zero smallest prime factor for composites */
        return spf[n];
    }

    /* Fallback for n > LIMIT: simple trial division */
    if ((n & 1) == 0) return 2;
    for (idx_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return d;
    }
    return 0;                      /* n is prime */
}
