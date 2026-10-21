/*
 * A081754  Numbers k such that the number of noncongruent solutions to
 *          x^(2^m) == 1 (mod k) is the same for any m>=1.
 * https://oeis.org/A081754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081754:
   a(n) = n-th positive integer k that is NOT divisible by 16
   and has no prime divisor congruent to 1 (mod 4).
   The sequence equals the set of integers whose prime factors are
   either 2 with exponent ≤ 3 or primes p ≡ 3 (mod 4). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache previously generated terms */
    static seq_t *cache = NULL;      /* array of terms */
    static idx_t cache_cnt = 0;      /* number of cached terms */

    if (n <= cache_cnt)              /* already computed */
        return cache[n-1];

    /* Determine an upper bound that will contain at least n terms.
       The density of allowed integers is ≈1/(log x), so n*10 is a safe
       initial guess; we double until enough terms are found. */
    idx_t limit = (n < 1000) ? 1000u : n * 10u;

    while (1) {
        /* ---------- sieve of Eratosthenes up to LIMIT ---------- */
        char *isP = (char *)malloc((limit + 1) * sizeof(char));
        if (!isP) return 0;          /* allocation failure */
        for (idx_t i = 0; i <= limit; ++i) isP[i] = 1;
        isP[0] = 0;  isP[1] = 0;
        for (idx_t i = 2; i <= limit / i; ++i) {
            if (isP[i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    isP[j] = 0;
            }
        }

        /* ---------- build the "allowed" array ---------- */
        char *allowed = (char *)malloc((limit + 1) * sizeof(char));
        if (!allowed) { free(isP); return 0; }
        for (idx_t i = 0; i <= limit; ++i) allowed[i] = 1;

        /* mark multiples of 16 */
        for (idx_t i = 16; i <= limit; i += 16)
            allowed[i] = 0;

        /* for every prime p ≡ 1 (mod 4), mark its multiples */
        for (idx_t p = 2; p <= limit; ++p) {
            if (isP[p] && (p & 3u) == 1u) {          /* p % 4 == 1 */
                for (idx_t j = p; j <= limit; j += p)
                    allowed[j] = 0;
            }
        }

        /* ---------- build new cache from allowed array ---------- */
        idx_t new_cnt = 0;
        for (idx_t i = 1; i <= limit; ++i)
            if (allowed[i]) ++new_cnt;

        if (new_cnt == 0) {                    /* nothing usable */
            free(isP);  free(allowed);
            limit <<= 1;                       /* enlarge limit */
            continue;
        }

        seq_t *new_cache = (seq_t *)realloc(cache, new_cnt * sizeof(seq_t));
        if (!new_cache) { free(isP); free(allowed); return 0; }
        cache = new_cache;

        idx_t idx = 0;
        for (idx_t i = 1; i <= limit; ++i)
            if (allowed[i]) cache[idx++] = (seq_t)i;

        cache_cnt = new_cnt;
        free(isP); free(allowed);

        if (cache_cnt >= n) break;            /* enough terms obtained */
        limit <<= 1;                           /* enlarge and retry */
    }

    return cache[n-1];
}
