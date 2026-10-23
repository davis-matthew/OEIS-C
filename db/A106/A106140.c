/*
 * A106140  Semiprimes (mod 4).
 * https://oeis.org/A106140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th semiprime modulo 4.
   A semiprime is a product of two primes (not necessarily distinct). */
#include <stdlib.h>
#include <string.h>

/* External declaration supplied by the build system.  Do not modify. */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Initial bound: 20*n+100, at least 200.  This covers up to
       several thousands of semiprimes comfortably. */
    uint64_t R = (uint64_t)n * 20ULL + 100ULL;
    if (R < 200ULL) R = 200ULL;

    while (1) {
        size_t sz = (size_t)(R + 1);
        uint32_t *spf = (uint32_t *)malloc(sz * sizeof(uint32_t));
        if (!spf) return 0;                     /* allocation failure */
        memset(spf, 0, sz * sizeof(uint32_t));

        /* Linear sieve to fill spf[] and prime list. */
        uint32_t *primes = (uint32_t *)malloc(sz * sizeof(uint32_t));
        if (!primes) { free(spf); return 0; }
        size_t pc = 0;

        for (size_t i = 2; i <= R; ++i) {
            if (!spf[i]) {
                spf[i] = (uint32_t)i;
                primes[pc++] = (uint32_t)i;
            }
            for (size_t j = 0; j < pc; ++j) {
                uint32_t p = primes[j];
                uint64_t v = (uint64_t)i * p;
                if (v > R || p > spf[i]) break;
                spf[(size_t)v] = p;
            }
        }

        /* Count semiprimes until we reach the n‑th one. */
        idx_t count = 0;
        seq_t result = 0;  /* will hold the modulo‑4 value */

        for (uint32_t num = 4; num <= (uint32_t)R; ++num) {
            uint32_t t = num;
            idx_t factors = 0;
            while (t > 1 && factors < 3) {
                uint32_t f = spf[t];
                t /= f;
                ++factors;
            }
            if (factors == 2 && t == 1) {   /* num is a semiprime */
                ++count;
                if ((idx_t)count == n) {
                    result = (seq_t)(num & 3U); /* num % 4 */
                    break;
                }
            }
        }

        free(primes);
        free(spf);

        if (count >= n) return result;   /* we found the n‑th semiprime */

        /* Not enough; increase the search bound and try again. */
        R *= 2;
    }
}
