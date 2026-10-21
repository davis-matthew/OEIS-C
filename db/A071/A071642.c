/*
 * A071642  Numbers n such that x^n + x^(n-1) + x^(n-2) + ... + x + 1 is
 *          irreducible over GF(2).
 * https://oeis.org/A071642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n = 1, else p-1 where p is the (n-1)-th prime such
 *            that 2 is a primitive root modulo p. Thus a(1)=0, a(2)=1 (p=2),
 *            a(3)=2 (p=3), a(4)=4 (p=5),
 * Range:     u8 n <= 26, u16 n <= 2445, u32 n <= 3605, u64 n <= 3605,
 *            u128 n <= 3605, big any n
 * Helpers:   incr_seq_cap, incr_uint64_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/incr_seq_cap.h"
#include "../utils/incr_uint64_cap.h"

/* Prime storage ----------------------------------------------- */
static uint64_t *prime_list = NULL;     /* list of primes found so far */
static size_t prime_count   = 0;        /* current number of primes   */
static size_t prime_cap     = 0;        /* capacity of prime_list     */

/* Primitive‑root–minus‑one values ---------------------------- */
static seq_t *prim_vals = NULL;         /* p-1 for each qualifying prime p */
static size_t prim_len  = 0;            /* number of values stored      */
static size_t prim_cap  = 0;            /* capacity of prim_vals        */

/* Next integer to examine for primality ---------------------- */
static uint64_t next_candidate = 2;

/* --------------------------------------------------------------------
   Modular exponentiation: (base^exp) mod mod, all 64‑bit integers.
   -------------------------------------------------------------------- */
static uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* --------------------------------------------------------------------
   Test whether a number is prime using the prime list.
   -------------------------------------------------------------------- */
static bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t q = prime_list[i];
        if (q * q > n) break;
        if (n % q == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------
   Check whether 2 is a primitive root modulo prime p.
   -------------------------------------------------------------------- */
static bool primitive_root_2(uint64_t p)
{
    if (p == 2) return true;
    uint64_t n = p - 1;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t q = prime_list[i];
        if (q * q > tmp) break;
        if (tmp % q == 0) {
            /* distinct prime factor q of n */
            if (powmod64(2, n / q, p) == 1) return false;
            while (tmp % q == 0) tmp /= q;
        }
    }
    /* remaining prime factor > sqrt(n) */
    if (tmp > 1) {
        if (powmod64(2, n / tmp, p) == 1) return false;
    }
    return true;
}

/* --------------------------------------------------------------------
   Ensure that prim_vals contains at least (need-1) values.
   -------------------------------------------------------------------- */
static void generate_prim_vals(size_t need)
{
    while (prim_len < (need - 1)) {
        /* Find next prime */
        while (!is_prime(next_candidate)) ++next_candidate;

        /* Store prime into prime_list */
        incr_uint64_cap(&prime_list, &prime_cap, prime_count + 1);
        prime_list[prime_count++] = next_candidate;

        /* Check primitive‑root property and store result */
        if (primitive_root_2(next_candidate)) {
            incr_seq_cap(&prim_vals, &prim_cap, prim_len + 1);
            prim_vals[prim_len++] = (seq_t)(next_candidate - 1);
        }

        ++next_candidate;
    }
}

/* --------------------------------------------------------------------
   Main function required by the benchmark.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;   /* the offset 1, first term is 0 */

    generate_prim_vals((size_t)n);
    /* a(2) corresponds to prim_vals[0] (p=2) */
    return prim_vals[(size_t)(n - 2)];
}
