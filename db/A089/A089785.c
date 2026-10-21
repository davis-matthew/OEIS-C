/*
 * A089785  a(n) is the least prime(k) such that the concatenation of prime(k)
 *          and prime(k-n) is prime.
 * https://oeis.org/A089785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest prime p = prime(k) for k ≥ n+1 such that
 *            concat(prime(k), prime(k−n)) is prime. The index k is 1‑based
 * Range:     u8 n <= 8, u16 n <= 222, u32 n <= 222, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"     /* void build_primes_upto(idx_t limit,
                                           uint64_t **primes_out,
                                           size_t *cnt_out); */
#include "../utils/is_prime_big.h"          /* bool is_prime_big(seq_t n); */
#include "../utils/concat_parts.h"          /* seq_t concat_parts(idx_t a,
                                         idx_t b); */

/* Prime table */
static uint64_t *prime_tbl = NULL;
static size_t prime_cnt   = 0;
static bool prime_init    = false;

/* Initialise the prime table up to a fixed limit (2 000 000). */
static void init_primes(void)
{
    if (prime_init)
        return;
    prime_init = true;

    const idx_t LIMIT = 2000000ULL;
    /* Allocate and build primes */
    build_primes_upto(LIMIT, &prime_tbl, &prime_cnt);
    /* If the build failed, prime_tbl will be NULL; callers will return 0. */
}

/* Primary function computing the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n < 1; safe guard */

    init_primes();
    if (prime_tbl == NULL)
        return 0;                    /* failed to initialise */

    /* Search for the smallest k ≥ n+1 such that the concatenation is prime. */
    for (idx_t k = n + 1; k <= prime_cnt; ++k) {
        /* Indices are 1‑based; array indices are 0‑based. */
        uint64_t pk   = prime_tbl[k - 1];
        uint64_t pk_n = prime_tbl[(k - n) - 1];

        /* Concatenate the two primes; both arguments fit in idx_t. */
        seq_t cat = concat_parts((idx_t)pk, (idx_t)pk_n);

        if (is_prime_big(cat)) {
            return (seq_t)pk;        /* cast to seq_t for the result type */
        }
    }

    /* If we run out of primes (should not happen for n ≤ 10000 with the
       chosen LIMIT), return 0 as a fallback. */
    return 0;
}
