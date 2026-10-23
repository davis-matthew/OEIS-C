/*
 * A175461  Semiprimes of form 8n+5.
 * https://oeis.org/A175461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Finds all semiprimes of the form 8k+5 via a sieve. a(n) is the
 *            n‑th such semiprime (offset 1)
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Static cache of computed values. */
static seq_t *semiprimes = NULL;
static size_t sem_count  = 0;
static size_t sem_cap    = 0;
static uint32_t *spf      = NULL;
static idx_t    spf_limit = 0;

/* One‑time initialization: build spf array and the list of semiprimes. */
static void init(void)
{
    if (spf_limit > 0)           /* already initialised */
        return;

    /* A limit large enough for the first 10 000 terms. */
    spf_limit = 2000000;
    spf = (uint32_t*)calloc((size_t)(spf_limit + 1), sizeof(uint32_t));
    for (idx_t i = 2; i <= spf_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if ((idx_t)i * (idx_t)i <= spf_limit) {
                for (idx_t j = i * i; j <= spf_limit; j += i) {
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
                }
            }
        }
    }

    /* Allocate a starting buffer for the semiprimes. */
    sem_cap = 20000;
    semiprimes = (seq_t*)malloc(sem_cap * sizeof(seq_t));

    /* Enumerate numbers congruent to 5 mod 8 and test for semiprime. */
    for (idx_t k = 0; ; ++k) {
        seq_t num = (seq_t)8 * (seq_t)k + (seq_t)5;
        if (num > (seq_t)spf_limit)
            break;

        uint32_t p = spf[(uint32_t)num];
        if (p == 0)
            continue;                  /* num is prime, not a semiprime */

        uint32_t q = (uint32_t)(num / p);
        if (q != 0 && spf[q] == (uint32_t)q) {
            /* q is prime (or q==p).  num is a semiprime. */
            if (sem_count == sem_cap) {
                sem_cap *= 2;
                semiprimes = (seq_t*)realloc(semiprimes,
                                            sem_cap * sizeof(seq_t));
            }
            semiprimes[sem_count++] = num;
        }
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1; undefined for n<1 */

    init();

    if (n <= sem_count)
        return semiprimes[n - 1];

    /* Requested index beyond precomputed range (should not happen for n≤10000). */
    return (seq_t)0;
}
