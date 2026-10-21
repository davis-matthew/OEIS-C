/*
 * A064548  Numbers k for which the sum of the binary digits equals the number
 *          of prime factors of k + 1 counted with multiplicity.
 * https://oeis.org/A064548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064548:
   a(n) = the n-th positive integer k such that
   popcount(k) = bigomega(k+1)
   (popcount = number of 1’s in binary expansion,
    bigomega = total number of prime factors counted with multiplicity)
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* ----- global tables for prime sieving ---------------------------------- */
static uint64_t *spf = NULL;          /* smallest prime factor for each number   */
static uint64_t  spf_len = 0;          /* size of the table (max index we have)   */
static uint32_t *primes = NULL;       /* list of primes found during sieving     */
static size_t    primes_len = 0;       /* number of primes stored                 */
static size_t    primes_cap = 0;       /* allocation capacity of primes array     */

#define MAX_SIEVE_LIMIT 4000000        /* limit we pre‑compute; covers n≈10000 */

/* ---- linear sieve: compute spf table up to 'limit' -------------------- */
static void build_spf(uint64_t limit)
{
    if (limit <= spf_len) return;            /* already covered */

    /* allocate new table */
    uint64_t *new_spf = (uint64_t*)malloc((limit + 1) * sizeof(uint64_t));
    if (!new_spf) abort();
    for (uint64_t i = 0; i <= limit; ++i) new_spf[i] = 0;

    /* allocate primes array */
    size_t new_cap = 100000;                 /* rough initial estimate   */
    uint32_t *new_primes = (uint32_t*)malloc(new_cap * sizeof(uint32_t));
    if (!new_primes) abort();

    /* linear sieve */
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!new_spf[i]) {
            new_spf[i] = i;
            if (primes_len == primes_cap) {
                primes_cap = primes_cap == 0 ? 64 : primes_cap * 2;
                new_primes = (uint32_t*)realloc(new_primes, primes_cap * sizeof(uint32_t));
                if (!new_primes) abort();
            }
            new_primes[primes_len++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_len; ++j) {
            uint32_t p = new_primes[j];
            uint64_t v = i * p;
            if (v > limit) break;
            new_spf[v] = p;
            if (p == new_spf[i]) break;
        }
    }

    /* free old data */
    if (spf) free(spf);
    if (primes) free(primes);

    spf = new_spf;
    spf_len = limit;
    primes = new_primes;
    primes_cap = new_cap;
}

/* ---- bigomega(x) using spf table ------------------------------------- */
static uint64_t bigomega(uint64_t x)
{
    uint64_t cnt = 0;
    while (x > 1) {
        uint64_t p = spf[x];
        while (x % p == 0) {
            x /= p;
            ++cnt;
        }
    }
    return cnt;
}

/* ---- sequence storage -------------------------------------------------- */
static seq_t *terms = NULL;        /* array of discovered terms (k values) */
static size_t terms_len = 0;       /* number of stored terms                */
static size_t terms_cap = 0;       /* current allocation capacity           */
static idx_t  next_k = 1;          /* next integer to test                   */
static bool    initialized = false;

/* ---- main sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but harmless */

    if (!initialized) {
        /* first call: precompute sieve and init storage */
        build_spf(MAX_SIEVE_LIMIT);
        terms_cap = 16;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
        terms_len = 0;
        next_k = 1;
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        assert((uint64_t)next_k + 1 <= spf_len);  /* we guaranteed enough sieve range */

        unsigned int pc = __builtin_popcountll((uint64_t)next_k);
        uint64_t bigO = bigomega((uint64_t)next_k + 1);

        if ((uint64_t)pc == bigO) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return terms[n - 1];
}
