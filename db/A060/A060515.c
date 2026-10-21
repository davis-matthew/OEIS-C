/*
 * A060515  Integers i > 1 for which there is no prime p such that i is a
 *          solution mod p of x^2 = 2.
 * https://oeis.org/A060515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A060515: a(n) = the n‑th integer i>1 such that all prime factors of i^2‑2 are ≤ i.
   The function generates terms in increasing order, caching the results. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- global state ---------- */
static size_t primes_cnt = 0;
static int *primes = NULL;
static bool primes_built = false;

static seq_t *cache = NULL;       /* cached sequence values */
static size_t cache_len = 0;      /* number of stored terms */
static uint64_t next_candidate = 2;   /* next integer to test */

/* ---------- prime sieve (once per program) ---------- */
static void build_primes(void)
{
    if (primes_built) return;

    /* limit must comfortably exceed all candidates that will appear for
       n up to about 10 000; 2 000 000 is more than enough.           */
    const size_t LIMIT = 2000000;
    bool *is_composite = (bool*)calloc(LIMIT + 1, sizeof(bool));
    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (!is_composite[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_composite[j] = true;

    /* count primes and store them */
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_composite[i]) ++primes_cnt;

    primes = (int*)malloc(primes_cnt * sizeof(int));
    size_t pos = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!is_composite[i]) primes[pos++] = (int)i;

    free(is_composite);
    primes_built = true;
}

/* ---------- required function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* offset is 1 – undefined for n<1 */

    build_primes();

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* search the next admissible i */
        for (;;) {
            uint64_t i = next_candidate++;
            if (i <= 1) continue;    /* i must be >1 */

            uint64_t m = i * i - 2ULL;   /* |i^2‑2|, fits in uint64_t for our n */
            uint64_t temp = m;
            bool ok = true;

            /* remove all prime factors ≤ i.  Any remaining factor > i
               immediately invalidates the candidate.                     */
            for (size_t idx = 0; idx < primes_cnt && (uint64_t)primes[idx] <= i; ++idx) {
                uint64_t p = (uint64_t)primes[idx];
                if (temp % p == 0) {
                    while (temp % p == 0) temp /= p;
                }
                if (temp == 1) break;
            }

            if (temp > 1) ok = false;    /* a prime factor > i was found */

            if (ok) {
                /* append i to the cache */
                cache = (seq_t*)realloc(cache, (cache_len + 1) * sizeof(seq_t));
                cache[cache_len++] = (seq_t)i;
                break;                  /* move on to fill the next cache slot */
            }
        }
    }
    return cache[n - 1];
}
