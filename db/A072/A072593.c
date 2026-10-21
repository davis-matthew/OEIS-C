/*
 * A072593  In prime factorization of n replace multiplication with bitwise
 *          logical 'or'.
 * https://oeis.org/A072593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bitwise OR of the distinct prime factors of n, a(1)=1
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The types seq_t and idx_t are provided by the build environment. */
#include <stdlib.h>
#include "../utils/build_spf.h"   /* provides: void build_spf(idx_t limit, idx_t **spf_out); */

/* Static cache for the smallest prime factor sieve. */
static idx_t *spf = NULL;       /* spf[i] = smallest prime factor of i (spf[0],spf[1] unused) */
static idx_t max_n = 0;         /* maximum n for which spf is valid */

/* Expand the sieve at least up to n, replacing the old array if necessary. */
static void ensure_spf(idx_t n)
{
    if (n <= max_n)
        return;                         /* already valid */

    idx_t *new_spf = NULL;
    build_spf(n, &new_spf);              /* build_spf allocates a new array [0..n] */

    if (spf && max_n > 0)
        free(spf);                       /* release the old array */

    spf = new_spf;
    max_n = n;
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined; return 0 as a safeguard */

    if (n == 1)
        return 1;                 /* a(1) is defined as 1 */

    ensure_spf(n);

    seq_t result = 0;
    idx_t x = n;

    while (x > 1) {
        idx_t p = spf[x];                /* smallest prime factor of x */
        result |= (seq_t)p;              /* OR the prime itself */
        while (x % p == 0)               /* skip repeated powers of this prime */
            x /= p;
    }

    return result;
}
