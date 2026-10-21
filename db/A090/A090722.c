/*
 * A090722  a(n) = if 10 - Mod(Prime(n),10) == {1,3,7,9} respectively then
 *          {1,2,3,0}.
 * https://oeis.org/A090722
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A090722: a(n) is obtained from the n‑th prime p=Prime(n).
   If p mod 10 equals 1,3,7,9, the output is 0,3,2,1 respectively.
   The function builds a list of primes up to the n‑th prime using a
   sieve (build_primes_upto) and caches the result. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;                 /* offset – behaviour below n=4 is irrelevant */

    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t last_limit = 0;

    /* Ensure we have at least n primes cached. */
    if (prime_cnt < (size_t)n) {
        idx_t limit = (n < 6) ? 15 : n * 20;   /* crude upper bound for the n‑th prime */
        while (1) {
            free(primes);                     /* free previous cache (free(NULL) safe) */
            build_primes_upto(limit, &primes, &prime_cnt);
            if (prime_cnt >= (size_t)n) break;
            limit *= 2;                       /* double the bound and retry */
        }
        last_limit = limit;
    }

    /* primes[0] = 2, so primes[n‑1] is Prime(n). */
    uint64_t p = primes[n - 1];
    uint64_t m = p % 10ULL;
    seq_t res;

    switch (m) {
        case 1: res = 0; break;   /* 1 → 0 */
        case 3: res = 3; break;   /* 3 → 3 */
        case 7: res = 2; break;   /* 7 → 2 */
        case 9: res = 1; break;   /* 9 → 1 */
        default: res = 0; break;  /* for primes 2,3,5 – never used for n≥4 */
    }
    return res;
}
