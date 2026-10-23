/*
 * A154301  Primes of the form (1+2+...+m)/75 = A000217(m)/75.
 * https://oeis.org/A154301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy, bref
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A154301: primes of the form (1+2+…+m)/75 = m(m+1)/150.
 *  The set of such primes is finite; for m>150 the value cannot be prime.
 *  This implementation pre‑computes all candidates once and stores the
 *  primes in a small static cache.  Subsequent calls return the cached
 *  value for the given n or 0 if n exceeds the number of known primes.
 */

#include <stdbool.h>
#include <stdint.h>

/* Provided by the framework for primality testing. */
#include "../utils/isPrimeGeneric.h"

/* static cache of the primes found */
static seq_t cache[4] = {0, 0, 0, 0};
static idx_t cache_count = 0;
static bool cache_initialized = false;

/* pre‑compute all primes of the form m(m+1)/150 for 1 ≤ m ≤ 150 */
static void precompute(void)
{
    const idx_t M_LIMIT = 150;
    for (idx_t m = 1; m <= M_LIMIT; ++m) {
        unsigned long long prod = m * (m + 1);            /* m(m+1)  */
        if (prod % 150 != 0) continue;                   /* not integral */
        unsigned long long val = prod / 150;             /* candidate prime */
        seq_t p = (seq_t)val;                            /* safe cast – val < 2^15 */
        if (isPrimeGeneric(p)) {
            if (cache_count < 4) {                       /* store only first 4 */
                cache[cache_count++] = p;
            } else {
                /* more primes would overflow our small cache; ignore */
            }
        }
    }
    cache_initialized = true;
}

/* ---------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* behaviour below the offset is irrelevant */

    if (!cache_initialized) precompute();

    /* n is 1‑based */
    if (n <= cache_count) return cache[n - 1];
    return (seq_t)0;                     /* no further terms */
}
