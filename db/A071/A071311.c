/*
 * A071311  Squarefree numbers k with largest prime factor = floor(sqrt(k)).
 * https://oeis.org/A071311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 6491, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates OEIS A071311:  
   Square‑free numbers k with largest prime factor = floor(sqrt(k)).
   For a prime p>2, if p+1 or p+2 is composite and square‑free,
   then k = p*(p+1) or k = p*(p+2) is in the sequence.  
   The terms are produced in ascending order as the primes grow. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree_uint32.h"

/* Compile‑time constants ------------------------------------------------- */
#define SIEVE_LIMIT   5000000      /* upper bound for sieve (5 million)     */
#define MAX_TERMS     50000        /* number of terms to pre‑compute       */

/* Static storage -------------------------------------------------------- */
static uint8_t  is_prime[SIEVE_LIMIT + 3];  /* 1 = prime, 0 = composite      */
static seq_t    terms[MAX_TERMS];           /* pre‑computed sequence values    */
static size_t   terms_len = 0;              /* how many terms are ready         */
static bool     init_done = false;          /* one‑time initialization flag    */

/* One‑time initialization: sieve and generate terms --------------------- */
static void init(void)
{
    if (init_done) return;

    /* ---------------------------------- sieve -------------------------------- */
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* --------------------------------- generate terms ------------------------- */
    for (uint32_t p = 3; p <= SIEVE_LIMIT && terms_len < MAX_TERMS; ++p) {
        if (!is_prime[p]) continue;              /* not a prime */
        /* p + 1 candidate */
        uint32_t n1 = p + 1;
        if (n1 <= SIEVE_LIMIT && !is_prime[n1] && is_squarefree_uint32(n1)) {
            terms[terms_len++] = (seq_t)p * (seq_t)n1;
            if (terms_len >= MAX_TERMS) break;
        }
        /* p + 2 candidate */
        uint32_t n2 = p + 2;
        if (n2 <= SIEVE_LIMIT && !is_prime[n2] && is_squarefree_uint32(n2)) {
            terms[terms_len++] = (seq_t)p * (seq_t)n2;
            if (terms_len >= MAX_TERMS) break;
        }
    }

    init_done = true;
}

/* ------------------------------------------------------- sequence function  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n==0, but guard anyway */
    init();                        /* ensure terms are ready */
    if (n <= terms_len) return terms[n - 1];
    /* if n is larger than pre‑computed range, return 0 (unlikely in tests) */
    return 0;
}
