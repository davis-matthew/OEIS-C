/*
 * A141077  Prime pairs of form (sum=2*3*5*7*11).
 * https://oeis.org/A141077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A141077 – prime pairs whose sum equals 2·3·5·7·11 = 2310.
 * a(2n‑1) = the n‑th prime p (p \< 1155) such that 2310‑p is also prime.
 * a(2n)   = 2310 - a(2n‑1).  There are exactly 456 terms (228 prime pairs). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/build_primes_upto.h"   /* provides build_primes_upto */

/* cache of the smaller primes in the admissible pairs  */
static uint64_t pair_primes[2311];
static size_t   pair_count   = 0;
static bool     initialized  = false;

/* initialise the cache of admissible prime pairs */
static void init(void)
{
    size_t       cnt   = 0;
    uint64_t    *pr    = NULL;
    uint64_t     p, q;

    /* get all primes up to 2310 (inclusive) */
    build_primes_upto(2310ULL, &pr, &cnt);

    /* build a small lookup table for primality up to 2310 */
    static bool isprime[2311] = { false };
    for (size_t i = 0; i < cnt; ++i)
        isprime[(size_t)pr[i]] = true;

    free(pr);

    /* collect the smaller prime of each admissible pair */
    pair_count = 0;
    for (p = 2; p <= 1155; ++p)
    {
        if (isprime[(size_t)p] &&
            isprime[(size_t)(2310ULL - p)])
        {
            pair_primes[pair_count++] = p;
        }
    }

    initialized = true;
}

/* return the n‑th term (n >= 1) of A141077 */
seq_t a(idx_t n)
{
    if (!initialized) init();

    /* total number of terms in the sequence (there are 228 primitive pairs) */
    const idx_t total = (idx_t)pair_count * 2;

    /* wrap indices larger than the sequence length – OEIS does not require this,
       but it keeps the function safe for arbitrary input. */
    if (n > total)
        n = ((n - 1) % total) + 1;

    /* 0‑based index in the combined list */
    const idx_t idx       = n - 1;                     /* 0‑based */
    const size_t pair_idx = (size_t)(idx >> 1);        /* which prime pair */
    const bool   is_small = (idx & 1) == 0;            /* is it the smaller prime? */

    if (is_small)
        return (seq_t)pair_primes[pair_idx];
    else
        return (seq_t)(2310ULL - pair_primes[pair_idx]);
}
