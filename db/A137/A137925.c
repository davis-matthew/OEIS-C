/*
 * A137925  a(n) = the largest divisor of n that is coprime to A002808(n).
 *          (A002808(n) = the n-th composite.)
 * https://oeis.org/A137925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest divisor of n coprime to the n‑th composite
 *            number (A002808(n))
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/factor_u64.h"

/* Global cache of composite numbers (A002808).  The array is 0‑based. */
static uint64_t *composite_list = NULL;
static size_t composite_count = 0;

/* ------------------------------------------------------------------
 * Build the list of composites up to the desired size.
 * The function grows the cache if necessary.
 * ------------------------------------------------------------------ */
static void ensure_composites(idx_t need)
{
    if (composite_count >= (size_t)need)
        return;                           /* enough already cached */

    /* Estimate an upper bound for the nth composite.  For n up to ~10⁴
     * this bound is conservative; for larger n it still works, as the
     * composite density is close to 1. */
    size_t limit = need * 5 + 100;
    if (limit < 200)
        limit = 200;

    /* Allocate or grow the composite array. */
    composite_list = (uint64_t *)realloc(composite_list,
                                         sizeof(uint64_t) * need);
    if (!composite_list)
        abort();

    /* Sieve of Eratosthenes up to 'limit' to identify primes. */
    unsigned char *is_prime = (unsigned char *)calloc(limit + 1, sizeof(unsigned char));
    if (!is_prime)
        abort();
    for (size_t i = 2; i <= limit; ++i)
        is_prime[i] = 1;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Collect the composites. */
    size_t count = 0;
    for (uint64_t num = 4; num <= limit && count < need; ++num) {
        if (!is_prime[(size_t)num]) {
            composite_list[count++] = num;
        }
    }
    free(is_prime);

    /* Store the new number of composites; they must be enough. */
    composite_count = count;
    if (composite_count < (size_t)need)
        abort();   /* the bound was insufficient (shouldn't happen) */
}

/* ------------------------------------------------------------------
 * Sequence function.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                     /* undefined for n=0 */

    /* Make sure we have the n‑th composite. */
    ensure_composites(n);
    uint64_t C = composite_list[n - 1];      /* nth composite number */

    /* Factorise n.  We use at most 64 prime factors for 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t    cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Build the largest divisor of n that is coprime with C. */
    seq_t result = (seq_t)1;                 /* start with 1 */
    for (size_t i = 0; i < cnt; ++i) {
        if (C % primes[i] != 0) {            /* prime does not divide C */
            seq_t power = seq_pow((seq_t)primes[i],
                                  (idx_t)exps[i]);    /* p^e */
            result = seq_mul(result, power);
        }
    }

    return result;
}
