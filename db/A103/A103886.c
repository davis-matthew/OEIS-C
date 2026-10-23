/*
 * A103886  Rearrangement of prime numbers p according to number of divisors
 *          of p-/+1.
 * https://oeis.org/A103886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(3k-2) = A103664(k) -- primes with τ(p-1) < τ(p+1) a(3k-1) =
 *            A067889(k) -- primes with τ(p-1) = τ(p+1) a(3k) = A103665(k) --
 *            primes with τ(p-1) > τ(p+1)
 * Range:     u8 n <= 19, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   add_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/add_seq.h"

/* --- data structures --------------------------------------- */
static uint64_t *prime_list = NULL;      /* primes used for trial division */
static idx_t   small_size   = 0;         /* current number of primes stored */
static idx_t   small_cap    = 0;         /* capacity of prime_list */

static seq_t  *type0 = NULL;             /* primes with τ(p-1) < τ(p+1) (A103664) */
static seq_t  *type1 = NULL;             /* primes with τ(p-1) = τ(p+1) (A067889) */
static seq_t  *type2 = NULL;             /* primes with τ(p-1) > τ(p+1) (A103665) */
static idx_t   sz0   = 0, sz1   = 0, sz2   = 0;  /* counts of stored primes */
static idx_t   cap0  = 0, cap1  = 0, cap2  = 0;  /* capacities of arrays */
static uint64_t next_candidate = 2;             /* next number to test for primality */

/* --- helper functions -------------------------------------- */

/* add a prime to the trial‑division list */
static void add_small_prime(uint64_t p) {
    if (small_cap == 0) {
        small_cap = 4;
        prime_list = (uint64_t *)malloc(small_cap * sizeof(uint64_t));
    }
    if (small_size == small_cap) {
        small_cap <<= 1;
        prime_list = (uint64_t *)realloc(prime_list, small_cap * sizeof(uint64_t));
    }
    prime_list[small_size++] = p;
}

/* trial division primality test */
static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    for (idx_t i = 0; i < small_size; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* number of divisors τ(n) for n ≥ 1 (using stored primes) */
static uint64_t tau_uint64(uint64_t n) {
    if (n == 1) return 1;
    uint64_t d = 1;
    uint64_t t = n;
    for (idx_t i = 0; i < small_size; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > t) break;
        if (t % p == 0) {
            unsigned e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            d *= (uint64_t)(e + 1);
        }
    }
    if (t > 1) d *= 2;
    return d;
}

/* generate primes until the three type arrays reach the required sizes */
static void ensure_sequences(idx_t need0, idx_t need1, idx_t need2) {
    while (sz0 < need0 || sz1 < need1 || sz2 < need2) {
        /* find the next prime */
        while (!is_prime(next_candidate)) ++next_candidate;
        uint64_t p = next_candidate;
        add_small_prime(p);

        /* classify the prime */
        uint64_t t1 = tau_uint64(p - 1);
        uint64_t t2 = tau_uint64(p + 1);
        if (t1 < t2)
            add_seq(&type0, &cap0, &sz0, (seq_t)p);
        else if (t1 > t2)
            add_seq(&type2, &cap2, &sz2, (seq_t)p);
        else
            add_seq(&type1, &cap1, &sz1, (seq_t)p);

        ++next_candidate;
    }
}

/* --- sequence function -------------------------------------- */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;         /* undefined for n < 1, return 0 */

    idx_t mod = n % 3;
    idx_t k0 = 0, k1 = 0, k2 = 0;
    if (mod == 1)          k0 = (n + 2) / 3;   /* a(3k-2) */
    else if (mod == 2)     k1 = (n + 1) / 3;   /* a(3k-1) */
    else                   k2 = n / 3;         /* a(3k)   */

    ensure_sequences(k0, k1, k2);

    if (mod == 1) return type0[k0 - 1];
    if (mod == 2) return type1[k1 - 1];
    return type2[k2 - 1];
}
