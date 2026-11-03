/*
 * A059641  Primes p such that x^52 = 2 has no solution mod p.
 * https://oeis.org/A059641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p (p > 2) for which the congruence * x^52
 *            ≡ 2 (mod p) has no solution. This is true iff *
 *            2^{(p‑1)/gcd(52,p‑1)} ̸≡ 1 (mod p). The function * uses a small
 *            incremental sieve together with the * utilities is_prime_small,
 *            mod_pow and gcd
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 801 (2026-11-03)
 * Helpers:   gcd, is_prime_seq, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_seq.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* static cache of the primes that satisfy the condition */
static seq_t *res        = NULL;   /* array of qualifying primes          */
static size_t res_cap    = 0;      /* current capacity of res            */
static size_t res_len    = 0;      /* number of elements in res          */

/* running position when generating new primes */
static uint64_t cand = 3;          /* next odd candidate to test         */

/* Return the n‑th such prime.  n ≥ 1. */
seq_t a(idx_t n)
{
    /* generate more primes until we have at least n entries */
    while (res_len < n)
    {
        /* test whether cand is prime */
        if (is_prime_seq(cand))
        {
            seq_t p   = (seq_t)cand;          /* candidate as seq_t */
            seq_t g   = gcd((seq_t)52, p - (seq_t)1);
            seq_t exp = (p - (seq_t)1) / g;   /* exponent for 2 */

            seq_t pow_val = powmod((seq_t)2, (uint64_t)exp, p);

            if (pow_val != (seq_t)1)          /* no 52nd‑power solution   */
            {
                /* append to cache */
                if (res_len == res_cap)
                    grow_seq(&res, &res_cap, res_len + 1);
                res[res_len++] = p;
            }
        }
        cand += 2;      /* skip even numbers */
    }

    return res[n - 1];
}
