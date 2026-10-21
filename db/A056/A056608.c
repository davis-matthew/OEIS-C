/*
 * A056608  Least prime factor of the n-th composite number.
 * https://oeis.org/A056608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = lpf(composite[n]) where composite[1] = 4, composite[2] =
 *            6,
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least prime factor of the n-th composite number */

#include <stdlib.h>
#include <stdint.h>

/* Forward declarations of global helpers (externally defined in utils) */
static seq_t *comp_lpf = NULL;   /* array of least prime factors of composites */
static uint32_t *spf = NULL;     /* smallest prime factor table */
static idx_t comp_count = 0;     /* number of composites stored */

/* Ensure that comp_lpf has at least n_needed entries */
static void ensure_sieve(idx_t n_needed)
{
    if (comp_count >= n_needed)
        return;

    /* Allocate a sieve big enough to obtain the required number of composites.
       n_needed*3+100 is generous for n <= 10000 and leaves room for growth. */
    idx_t limit = n_needed * 3 + 100;
    if (limit < 100)
        limit = 100;           /* minimum size */

    /* Allocate or reallocate the smallest prime factor table */
    uint32_t *new_spf = (uint32_t *)realloc(spf, (size_t)(limit + 1) * sizeof(uint32_t));
    if (!new_spf)
        return;                /* allocation failed – leave function */
    spf = new_spf;

    /* Initialize to 0 */
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = 0;

    /* Sieve of Eratosthenes: compute spf for every number up to limit */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if ((idx_t)i * (idx_t)i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
            }
        }
    }

    /* Count composites and build the comp_lpf array */
    idx_t new_count = 0;
    for (idx_t i = 4; i <= limit; ++i)
        if (spf[i] != i)
            ++new_count;

    seq_t *new_comp = (seq_t *)realloc(comp_lpf, (size_t)new_count * sizeof(seq_t));
    if (!new_comp)
        return;                /* allocation failed – leave function */
    comp_lpf = new_comp;

    idx_t idx = 0;
    for (idx_t i = 4; i <= limit; ++i) {
        if (spf[i] != i)      /* composite */
            comp_lpf[idx++] = (seq_t)spf[i];
    }

    comp_count = new_count;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n==0; offset is 1 */

    ensure_sieve(n);
    if (n <= comp_count)
        return comp_lpf[n - 1];
    return 0;                /* should never happen for valid input */
}
