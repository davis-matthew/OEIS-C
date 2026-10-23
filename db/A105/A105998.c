/*
 * A105998  Semiprime function n -> A001358(n) applied four times to n.
 * https://oeis.org/A105998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A001358(A001358(A001358(A001358(n))))
 * Range:     u8 n <= 4, u16 n <= 323, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   uint32_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/uint32_cmp.h"

/* maximum value needed for the 4‑fold semiprime mapping
   n ≤ 10 000  →  a(n) < 10⁷, 4th index < 1 000 000  */
#define SEMILIMIT 7000000      /* sieve limit (≥ 7 000 000) */
#define PRIMECAP   500000      /* enough for primes ≤ SEMILIMIT   */
#define SEMICAP    1500000     /* enough for semiprimes ≤ SEMILIMIT */

static int init_done = 0;
static uint8_t *is_composite = NULL;   /* 0 = prime, 1 = composite   */
static uint32_t *primes = NULL;         /* list of primes              */
static size_t prime_cnt = 0;
static uint32_t *semis = NULL;          /* semiprimes list             */
static size_t semis_cnt = 0;

/* one‑time precomputation of primes and semiprimes */
static void init(void)
{
    /* --- sieve of Eratosthenes up to SEMILIMIT --- */
    size_t i, j;
    is_composite = (uint8_t *)calloc(SEMILIMIT + 1, 1);
    primes = (uint32_t *)malloc(PRIMECAP * sizeof(uint32_t));
    semis  = (uint32_t *)malloc(SEMICAP * sizeof(uint32_t));

    for (i = 2; i <= SEMILIMIT; ++i) {
        if (!is_composite[i]) {
            if (prime_cnt < PRIMECAP) {
                primes[prime_cnt++] = (uint32_t)i;
            }
            if (i * i <= SEMILIMIT) {
                for (j = i * i; j <= SEMILIMIT; j += i) {
                    is_composite[j] = 1;
                }
            }
        }
    }

    /* --- generate semiprimes p*q with p ≤ q --- */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint32_t p = primes[pi];
        for (size_t qi = pi; qi < prime_cnt; ++qi) {
            uint64_t prod = (uint64_t)p * primes[qi];
            if (prod > SEMILIMIT) break;
            if (semis_cnt < SEMICAP) {
                semis[semis_cnt++] = (uint32_t)prod;
            }
        }
    }

    /* sort the semiprimes (they are not guaranteed to be in order) */
    qsort(semis, semis_cnt, sizeof(uint32_t), uint32_cmp);

    init_done = 1;
}

/* semiprime function: the n-th semiprime (n ≥ 1) */
static uint32_t semiprime(idx_t n)
{
    return semis[(size_t)(n - 1)];
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (!init_done) init();

    /* evaluate s(s(s(s(n)))) where s = semiprime() */
    uint32_t val = (uint32_t)n;
    for (int i = 0; i < 4; ++i) {
        val = semiprime(val);
    }
    return (seq_t)val;
}
