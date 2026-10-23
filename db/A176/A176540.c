/*
 * A176540  1 together with the semiprimes.
 * https://oeis.org/A176540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(1)=1, a(n) for n≥2 is the (n‑2)-th semiprime (product of two
 *            primes, counted with multiplicity)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   grow_limit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/grow_limit.h"

/* Return the n-th term of A176540. */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    /* static state to allow non‑sequential calls   */
    static seq_t *semis      = NULL;     /* array of found semiprimes   */
    static size_t semis_cnt  = 0;         /* number of stored semiprimes */
    static uint64_t limit    = 4;         /* current sieve limit         */
    static uint64_t prev_limit = 0;       /* previous sieve limit        */

    /* We need target semiprime index beginning at 0 for value 4. */
    size_t target = (size_t)(n - 2);

    /* Grow the sieve and isolate new semiprimes until we have enough. */
    while (semis_cnt <= target) {
        grow_limit(&limit);              /* expand the upper bound */

        /* Sieve of Eratosthenes with smallest prime factor (linear sieve). */
        uint64_t *spf = (uint64_t *)calloc((size_t)limit + 1, sizeof(uint64_t));
        if (!spf) return (seq_t)0;       /* memory failure */

        /* An array for primes during linear sieve. */
        size_t primes_cap = (size_t)limit / 10 + 10;
        uint32_t *primes   = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
        if (!primes) { free(spf); return (seq_t)0; }

        size_t primes_cnt = 0;
        for (uint64_t i = 2; i <= limit; ++i) {
            if (!spf[i]) {
                spf[i] = i;
                if (primes_cnt == primes_cap) { /* resize if needed */
                    primes_cap <<= 1;
                    uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                    if (!tmp) { free(primes); free(spf); return (seq_t)0; }
                    primes = tmp;
                }
                primes[primes_cnt++] = (uint32_t)i;
            }
            for (size_t j = 0; j < primes_cnt; ++j) {
                uint64_t p = primes[j];
                if (p > spf[i] || i * p > limit) break;
                spf[i * p] = p;
            }
        }

        /* Count prime factors with multiplicity for numbers 2..limit. */
        uint8_t *fc = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
        if (!fc) { free(primes); free(spf); return (seq_t)0; }

        for (uint64_t i = 2; i <= limit; ++i) {
            fc[i] = fc[i / spf[i]] + 1;
        }

        /* Count how many new semiprimes appear beyond the previous limit. */
        size_t new_in_this_pass = 0;
        for (uint64_t i = prev_limit + 1; i <= limit; ++i) {
            if (fc[i] == 2) ++new_in_this_pass;
        }

        /* Grow the storage for semiprimes. */
        seq_t *new_semis = (seq_t *)realloc(semis, (semis_cnt + new_in_this_pass) * sizeof(seq_t));
        if (!new_semis) { free(fc); free(primes); free(spf); return (seq_t)0; }
        semis = new_semis;

        /* Store the new semiprimes. */
        size_t idx = semis_cnt;
        for (uint64_t i = prev_limit + 1; i <= limit; ++i) {
            if (fc[i] == 2) {
                semis[idx++] = (seq_t)i;
            }
        }
        semis_cnt = idx;
        prev_limit = limit;

        free(fc);
        free(primes);
        free(spf);
    }

    /* Return the requested term (after the initial 1). */
    return semis[n - 2];
}
