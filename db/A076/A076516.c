/*
 * A076516  Primes p such that (p-1) and the period length of 1/p are both
 *          squares.
 * https://oeis.org/A076516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 230, u64 n <= 230,
 *            u128 n <= 230, big any n
 * Helpers:   factor_u64, floor_sqrt, is_prime_small, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A076516: primes p such that (p‑1) and the decimal period
   length of 1/p are both perfect squares.  The primes are searched as
   p = k² + 1 for k ≥ 2; for each prime found we compute the
   multiplicative order of 10 modulo p and accept p only if that
   order is a square.  The result is memoised and works for any
   <:idx_t> and <:seq_t> supplied by the build. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t n); */
#include "../utils/mod_pow.h"          /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/floor_sqrt.h"      /* seq_t floor_sqrt(seq_t x); */
#include "../utils/factor_u64.h"      /* void factor_u64(uint64_t n, uint64_t *primes, uint64_t *exps, size_t *cnt); */

static seq_t *terms   = NULL;   /* stored primes */
static size_t terms_len = 0;    /* number of primes found so far */
static size_t terms_cap = 0;    /* allocated capacity of the array */
static uint64_t next_k  = 2;    /* next value of k to test (k = 2 gives p = 5) */

/* Ensure the dynamic array has capacity for at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = new_arr;
    terms_cap = new_cap;
}

/* Main function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for zero and negative indices */

    /* Generate terms until we have at least n of them. */
    while (terms_len < (size_t)n) {
        uint64_t k     = next_k++;
        uint64_t p_num = k * k + 1ULL;

        /* Ignore numbers that are too small or obviously not primes. */
        if (p_num < 5ULL)
            continue;
        if (!is_prime_small((seq_t)p_num))
            continue;

        /* Compute the multiplicative order of 10 mod p. */
        uint64_t order = p_num - 1ULL;        /* p-1 = k², guaranteed square */

        /* Factor p-1 (which is ≤ 64‑bit). */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t pcnt = 0;
        factor_u64(order, primes, exps, &pcnt);

        /* Reduce 'order' by each prime factor as much as possible. */
        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t q = primes[i];
            for (uint64_t cnt = 0; cnt < exps[i]; ++cnt) {
                if (order % q != 0)
                    break;
                uint64_t cand = order / q;
                if (mod_pow((seq_t)10, cand, (seq_t)p_num) == (seq_t)1)
                    order = cand;
                else
                    /* further division by the same prime would increase the exponent */
                    break;
            }
        }

        /* Check whether the found order is a perfect square. */
        seq_t order_seq   = (seq_t)order;
        seq_t sqrt_order  = floor_sqrt(order_seq);
        if (sqrt_order * sqrt_order == order_seq) {
            /* Store the qualifying prime. */
            ensure_capacity(terms_len + 1);
            terms[terms_len++] = (seq_t)p_num;
        }
    }

    return terms[n - 1];
}
