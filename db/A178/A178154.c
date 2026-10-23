/*
 * A178154  Sum_{a=1..(p-1)/2} LegendreSymbol(a, p) for primes p == 3 (mod 4).
 * https://oeis.org/A178154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{a=1..(p-1)/2} LegendreSymbol(a,p) * where p is the nth
 *            prime ≡ 3 (mod 4) (OEIS A178154)
 * Range:     u8 n <= 1271, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* ----  Internal state for cached primes ≡ 3 (mod 4)  ---- */
static uint64_t *primes3     = NULL;  /* array of primes 3 mod 4   */
static size_t   primes3_cnt  = 0;     /* number of cached primes   */
static size_t   primes3_cap  = 0;     /* allocated capacity        */
static uint64_t sieve_limit  = 0;     /* upper bound of last sieve */

/* --- Recompute all primes 3 mod 4 up to 'limit' and store them in primes3. */
static void recompute_primes3(uint64_t limit)
{
    free(primes3);
    primes3 = NULL;
    primes3_cnt = 0;
    primes3_cap = 0;

    size_t sz = (size_t)limit + 1;
    char *comp = (char *)calloc(sz, sizeof(char));   /* C++ cast for void* */
    if (!comp) return;   /* allocation failed – nothing to do */

    /* simple Eratosthenes sieve up to limit */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (!comp[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                comp[j] = 1;
        }
    }

    for (size_t i = 3; i <= limit; i += 2) {
        if (!comp[i] && (i % 4 == 3)) {
            if (primes3_cnt >= primes3_cap) {
                size_t newcap = (primes3_cap == 0) ? 64 : primes3_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(primes3, newcap * sizeof(uint64_t)); /* C++ cast */
                if (!tmp) break;   /* stop if realloc fails */
                primes3 = tmp;
                primes3_cap = newcap;
            }
            primes3[primes3_cnt++] = i;
        }
    }

    free(comp);
    sieve_limit = limit;
}

/* ---- Ensure that primes3_cnt ≥ n by increasing sieve_limit if necessary. ---- */
static void ensure_primes(idx_t n)
{
    if (primes3_cnt >= (size_t)n) return;

    uint64_t limit = (sieve_limit != 0) ? sieve_limit : 20;
    while (primes3_cnt < (size_t)n) {
        limit = limit * 2 + 100;          /* enlarge bound */
        recompute_primes3(limit);
        if (primes3_cnt >= (size_t)n) break;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1 – ignore n==0 */
    ensure_primes(n);

    uint64_t p   = primes3[n - 1];      /* nth prime ≡3 (mod 4) */
    uint64_t half = (p - 1) / 2;        /* (p‑1)/2 */

    /* Count how many a satisfy LegendreSymbol(a,p)=+1 among 1..half. */
    int64_t count = 0;
    for (uint64_t a = 1; a <= half; ++a) {
        uint64_t val = (a * a) % p;
        if (val <= half) ++count;
    }

    int64_t sum = 2 * count - (int64_t)half;   /* sum of Legendre symbols */
    return (seq_t)sum;
}
