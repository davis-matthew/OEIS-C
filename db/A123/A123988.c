/*
 * A123988  Primes p such that 2^x == 3 (mod p) has no solutions.
 * https://oeis.org/A123988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that 2^x ≡ 3 (mod p) has no solution
 * Range:     u8 n <= 22, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   factor_u64, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm tests primes in ascending order.
   For each prime p, the discrete logarithm of 3 base 2 exists
   iff 3^(ord_p(2)) ≡ 1 mod p, where ord_p(2) is the multiplicative
   order of 2 modulo p.  We compute ord_p(2) by factoring p-1 and
   reducing the candidate order with modular exponentiation.
   Primes that fail the test are stored in a memoised array so that
   calls to a(idx_t n) for arbitrary n are fast. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

/* memoised sequence table */
static seq_t *prime_seq   = NULL;  /* array of primes satisfying the property */
static size_t seq_len     = 0;     /* number of elements currently stored    */
static size_t seq_cap     = 0;     /* allocated capacity of prime_seq         */
static uint64_t next_candidate = 3;/* next number to test (odd only)          */

/* compute the multiplicative order of 2 modulo prime p */
static uint64_t order_of_two(uint64_t p)
{
    uint64_t ord = p - 1;

    /* factor p-1 */
    uint64_t factors[64];
    uint64_t exps[64];
    size_t nf = 0;
    factor_u64(p - 1, factors, exps, &nf);

    /* try to reduce ord by each prime factor */
    for (size_t i = 0; i < nf; ++i)
    {
        uint64_t q = factors[i];
        while (ord % q == 0)
        {
            uint64_t cand = ord / q;
            if (mod_pow((seq_t)2, cand, (seq_t)p) == (seq_t)1)
                ord = cand;
            else
                break;
        }
    }
    return ord;
}

/* does 2^x ≡ 3 (mod p) have a solution?  (true -> solution exists) */
static bool has_solution(uint64_t p)
{
    uint64_t ord = order_of_two(p);
    return mod_pow((seq_t)3, ord, (seq_t)p) == (seq_t)1;
}

/* grow the memoised array when capacity is reached */
static void grow_seq_array(void)
{
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) abort();  /* out‑of‑memory */
    memcpy(new_arr, prime_seq, seq_len * sizeof(seq_t));
    free(prime_seq);
    prime_seq = new_arr;
    seq_cap   = new_cap;
}

/* ensure that at least n terms are stored in prime_seq */
static void ensure_n_terms(idx_t n)
{
    if (seq_len >= (size_t)n)
        return;

    if (!prime_seq)
    {
        seq_cap = 64;
        prime_seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!prime_seq) abort();
    }

    while (seq_len < (size_t)n)
    {
        /* skip even numbers */
        if ((next_candidate & 1) == 0)
        {
            ++next_candidate;
            continue;
        }

        /* test primality */
        if (is_prime((idx_t)next_candidate))
        {
            if (!has_solution(next_candidate))
            {
                /* store the prime */
                if (seq_len == seq_cap)
                    grow_seq_array();
                prime_seq[seq_len++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }
}

/* public OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n = 0 */
    ensure_n_terms(n);
    return prime_seq[(size_t)n - 1];
}
