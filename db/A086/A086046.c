/*
 * A086046  Sum of first n 4-almost primes.
 * https://oeis.org/A086046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of the first n 4‑almost primes
 * Range:     u8 n <= 6, u16 n <= 136, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* defined only for n >= 1 */

    /* Cache of cumulative sums of 4‑almost primes. */
    static seq_t *cum = NULL;   /* array of sums */
    static size_t cum_cap = 0;  /* allocated capacity */
    static idx_t  cum_len = 0;  /* number of terms stored */

    /* If we already know the answer, just return it. */
    if (cum_len >= n)
        return cum[n - 1];

    /* ------------------------------------------------------------------
       Find an upper bound for the n‑th 4‑almost prime.
       ------------------------------------------------------------------ */
    idx_t limit = 1000;                     /* starting limit */
    while (1) {
        uint32_t *spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
        uint8_t  *omega = (uint8_t  *)calloc((size_t)limit + 1, sizeof(uint8_t));
        if (!spf || !omega) {
            free(spf); free(omega);
            return (seq_t)0;                /* allocation failure */
        }

        omega[1] = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (spf[i] == 0) {               /* i is prime */
                spf[i] = (uint32_t)i;
                if (i * i <= limit) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
            omega[i] = omega[i / (idx_t)spf[i]] + 1;   /* Ω(i) */
        }

        idx_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (omega[i] == 4)
                ++cnt;

        free(spf); free(omega);

        if (cnt >= n)
            break;                    /* sufficient terms found */

        limit *= 2;                    /* increase bound */
    }

    /* ------------------------------------------------------------------
       Compute the first n 4‑almost primes and their cumulative sums.
       ------------------------------------------------------------------ */
    /* Sieve again to gather the numbers. */
    uint32_t *spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    uint8_t  *omega = (uint8_t  *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!spf || !omega) {
        free(spf); free(omega);
        return (seq_t)0;                /* allocation failure */
    }

    omega[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
        omega[i] = omega[i / (idx_t)spf[i]] + 1;
    }

    seq_t *new_cum = (seq_t *)calloc((size_t)n, sizeof(seq_t));
    if (!new_cum) {
        free(spf); free(omega);
        return (seq_t)0;
    }

    seq_t sum = 0;
    idx_t pos = 0;
    for (idx_t i = 2; i <= limit && pos < n; ++i) {
        if (omega[i] == 4) {
            sum += (seq_t)i;
            new_cum[pos++] = sum;
        }
    }

    free(spf);
    free(omega);

    /* Replace old cache. */
    free(cum);
    cum = new_cum;
    cum_cap = (size_t)n;
    cum_len = n;
    return cum[n - 1];
}
