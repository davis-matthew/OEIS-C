/*
 * A190105  a(n) = (3*A002145(n) - 1)/4.
 * https://oeis.org/A190105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*A002145(n) - 1)/4
 * Range:     u8 n <= 29, u16 n <= 3912, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (A002145(n) is the nth prime congruent to 3 (mod 4)) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* ---------- local static state ---------- */
static uint64_t *primes_4k3 = NULL;        /* array of cached primes */
static size_t primes_4k3_cnt = 0;          /* amount of primes stored */
static idx_t  primes_4k3_limit = 0;       /* largest index sieved */

/* ---------- helper to estimate upper bound for nth prime ≡3 mod 4 ---------- */
static idx_t estimate_limit(idx_t n)
{
    if (n < 6) return 15;                  /* trivial small cases */
    double nd = (double)n;
    /* Rough upper bound: 3*n*(log n + log log n) + 10, safety factor 3 */
    double r = nd * (log(nd) + log(log(nd))) * 3.0 + 10.0;
    return (idx_t)r;
}

/* ---------- rebuild cache up to at least n primes ≡3 mod 4 ---------- */
static void ensure_primes_4k3(idx_t n)
{
    if (primes_4k3_cnt >= n) return;              /* already enough */

    /* Determine new sieve limit */
    idx_t new_limit = estimate_limit(n);
    if (new_limit <= primes_4k3_limit) new_limit = primes_4k3_limit * 2 + 10;

    /* Allocate temporary sieve (uint8_t used as boolean flag) */
    unsigned char *sieve = (unsigned char *)malloc(new_limit + 1);
    if (!sieve) return;                           /* allocation failure */

    /* Initialise sieve */
    for (idx_t i = 0; i <= new_limit; ++i) sieve[i] = 1;
    if (new_limit >= 0) sieve[0] = 0;
    if (new_limit >= 1) sieve[1] = 0;

    /* Classic Eratosthenes */
    for (idx_t i = 2; i * i <= new_limit; ++i)
        if (sieve[i])
            for (idx_t j = i * i; j <= new_limit; j += i)
                sieve[j] = 0;

    /* Rebuild the list of primes congruent to 3 (mod 4) */
    size_t new_cap = 16;
    uint64_t *new_arr = (uint64_t *)malloc(new_cap * sizeof(uint64_t));
    size_t new_cnt = 0;

    for (idx_t i = 2; i <= new_limit; ++i)
        if (sieve[i] && (i & 3) == 3) {
            if (new_cnt == new_cap) {
                new_cap *= 2;
                new_arr = (uint64_t *)realloc(new_arr, new_cap * sizeof(uint64_t));
            }
            new_arr[new_cnt++] = (uint64_t)i;
        }

    /* Replace old cache */
    free(primes_4k3);
    primes_4k3 = new_arr;
    primes_4k3_cnt = new_cnt;
    primes_4k3_limit = new_limit;

    free(sieve);
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                            /* undefined for n < 1 */

    /* Ensure we have the n‑th prime ≡3 (mod 4) */
    ensure_primes_4k3(n);

    uint64_t p = primes_4k3[n - 1];
    seq_t res = ((seq_t)3 * (seq_t)p - (seq_t)1) / (seq_t)4;
    return res;
}
