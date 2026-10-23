/*
 * A130851  Catalan numbers A000108(n) modulo 9.
 * https://oeis.org/A130851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = Catalan(n) mod 9
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   exp3_fact, pow_mod9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = (1/(n+1)) * binom(2n,n)  (mod 9)  */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/exp3_fact.h"
#include "../utils/pow_mod9.h"

/* ----------  cache for factorials modulo 9 with all 3‑factors removed ---------- */
static uint8_t *fact_no3_cache = NULL;
static idx_t fact_cache_size   = 0;

/* ensure fact_no3_cache holds values up to n (inclusive) */
static void ensure_fact_cache(idx_t n)
{
    if (fact_cache_size > n)
        return;

    idx_t new_size = n + 1;
    uint8_t *tmp = (uint8_t *)realloc(fact_no3_cache, new_size * sizeof(uint8_t));
    if (!tmp) abort();                /* memory allocation failure */
    fact_no3_cache = tmp;

    /* initialise from previous size */
    idx_t i = fact_cache_size;
    uint8_t prod = 0;
    if (i == 0) {
        fact_no3_cache[0] = 1 % 9;   /* 0! = 1, no factor 3 */
        prod = fact_no3_cache[0];
        i = 1;
    } else {
        prod = fact_no3_cache[i - 1];
    }

    for (; i < new_size; ++i)
    {
        idx_t x = i;
        while (x % 3 == 0)          /* remove all factors of 3 */
            x /= 3;
        prod = (prod * (uint8_t)(x % 9)) % 9;
        fact_no3_cache[i] = prod;
    }
    fact_cache_size = new_size;
}

/* ----------  main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Catalan(0) = 1 */
    if (n == 0)
        return (seq_t)1 % 9;

    /* need factorials up to 2n with 3‑factors removed */
    ensure_fact_cache(2 * n);

    uint8_t prod_2n = fact_no3_cache[2 * n];
    uint8_t prod_n  = fact_no3_cache[n];

    /* inverse of prod_n modulo 9 (coprime to 3) */
    uint8_t inv_n = pow_mod9(prod_n, 5);   /* because a^6 ≡ 1 (mod 9) for units */

    /* binom(2n,n) modulo 9 ignoring 3‑factors */
    uint8_t binom = prod_2n;
    binom = (binom * inv_n) % 9;
    binom = (binom * inv_n) % 9;

    /* exponent of 3 in binom(2n,n) */
    idx_t exp3_binom = exp3_fact(2 * n) - 2 * exp3_fact(n);

    /* handle division by (n+1) */
    idx_t denom = n + 1;
    idx_t exp3_denom = 0;
    while (denom % 3 == 0)
    {
        denom /= 3;
        ++exp3_denom;
    }
    idx_t exp3 = exp3_binom - exp3_denom;

    /* if 3² divides Catalan(n), result is 0 modulo 9 */
    if (exp3 >= 2)
        return (seq_t)0;

    /* inverse of the remaining part of (n+1) modulo 9 */
    uint8_t rem = (uint8_t)(denom % 9);
    uint8_t inv_rem = pow_mod9(rem, 5);

    /* final value modulo 9 */
    uint8_t val = (binom * inv_rem) % 9;
    if (exp3 == 1)           /* multiply by one factor of 3 */
        val = (val * 3) % 9;

    return (seq_t)val;
}
