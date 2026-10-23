/*
 * A167794  Numbers with primitive root 6.
 * https://oeis.org/A167794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers with primitive root 6.
    A(n) is the nth odd integer coprime to 6 for which 6 is a primitive root
    modulo the integer.  The function enumerates candidates, tests the
    necessary conditions (prime case: 6 is of order p‑1; prime‑power case:
    6 is a primitive root for the base prime and the lift condition
    6^(p‑1) mod p² ≠ 1), and stores the results in a dynamic array. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"

/* ---------- prime sieve for trial division ---------- */
static uint64_t *prime_tbl = NULL;   /* array of primes */
static size_t prime_cnt  = 0;       /* number of primes loaded */
static size_t prime_cap  = 0;       /* allocated size */

/* grow the prime table to at least new_cap slots */
static void grow_prime_tbl(size_t new_cap)
{
    if (new_cap <= prime_cap) return;
    size_t cap = prime_cap ? prime_cap : 16;
    while (cap < new_cap) cap *= 2;
    uint64_t *tmp = (uint64_t*)realloc(prime_tbl, cap * sizeof(uint64_t));
    prime_tbl = tmp;
    prime_cap = cap;
}

/* extend the prime table up to and including `limit` (>=2) */
static void ensure_primes_upto(uint64_t limit)
{
    if (prime_cnt == 0) {          /* first prime */
        grow_prime_tbl(16);
        prime_tbl[0] = 2;
        prime_cnt   = 1;
    }
    uint64_t last = prime_tbl[prime_cnt - 1];
    if (last >= limit) return;

    uint64_t candidate = last + 1;
    while (candidate <= limit) {
        bool is_p = true;
        uint64_t sq = (uint64_t)sqrt((long double)candidate);
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_tbl[i];
            if (p > sq) break;
            if (candidate % p == 0) { is_p = false; break; }
        }
        if (is_p) {
            grow_prime_tbl(prime_cnt + 1);
            prime_tbl[prime_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* ---------- sequence term storage ---------- */
static seq_t *term_arr   = NULL;   /* array of terms */
static size_t term_cnt   = 0;      /* number of terms stored */
static size_t term_cap   = 0;      /* allocated capacity */
static uint64_t next_cand = 5;     /* next odd candidate (first odd ≠ 3) */

/* grow the term array if needed */
static void ensure_term_cap(size_t needed)
{
    if (term_cap >= needed) return;
    while (term_cap < needed) term_cap = term_cap ? term_cap * 2 : 16;
    seq_t *tmp = (seq_t*)realloc(term_arr, term_cap * sizeof(seq_t));
    term_arr = tmp;
}

/* append a term to the array */
static void push_term(seq_t val)
{
    ensure_term_cap(term_cnt + 1);
    term_arr[term_cnt++] = val;
}

/* ---------- helper functions ---------- */

/* 6 is a primitive root mod an odd prime p > 2 */
static bool is_root_prime(uint64_t p)
{
    /* p must be prime – caller guarantees this */
    uint64_t ph = p - 1;                 /* φ(p) */
    ensure_primes_upto(ph);
    uint64_t tmp = ph;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = prime_tbl[i];
        if (q * q > tmp) break;
        if (tmp % q == 0) {
            seq_t v = mod_pow((seq_t)6, (uint64_t)(ph / q), (seq_t)p);
            if (v == 1) return false;
            while (tmp % q == 0) tmp /= q;
        }
    }
    if (tmp > 1) {
        seq_t v = mod_pow((seq_t)6, (uint64_t)(ph / tmp), (seq_t)p);
        if (v == 1) return false;
    }
    return true;
}

/* test whether a composite odd number m (not divisible by 3) is a prime power
   of a prime p with 6 as a primitive root, and the lift condition holds */
static bool is_prime_power_with_root(uint64_t m)
{
    /* factor the smallest prime divisor of m */
    uint64_t p = 0;
    ensure_primes_upto((uint64_t)sqrt((long double)m) + 2);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t q = prime_tbl[i];
        if (q * q > m) break;
        if (m % q == 0) { p = q; break; }
    }
    if (p == 0) return false;            /* m is prime */

    /* power test: m = p^k? */
    uint64_t tmp = m;
    unsigned exp = 0;
    while (tmp % p == 0) { tmp /= p; ++exp; }
    if (tmp != 1 || exp < 2) return false;   /* not a pure prime power */

    /* check that p itself has 6 as a primitive root */
    if (!is_root_prime(p)) return false;

    /* lift condition: 6^(p-1) mod p^2 != 1 */
    seq_t mod = (seq_t)p * (seq_t)p;
    seq_t v = mod_pow((seq_t)6, (uint64_t)(p - 1), mod);
    return v != 1;
}

/* does 6 have primitive root modulo m? (m odd, not divisible by 3) */
static bool has_root6(uint64_t m)
{
    if (m % 2 == 0 || m % 3 == 0) return false;
    if (is_prime((idx_t)m)) return is_root_prime(m);
    return is_prime_power_with_root(m);
}

/* generate terms until we have at least `target` terms */
static void ensure_terms_up_to(idx_t target)
{
    while (term_cnt < target) {
        /* advance to next odd number not divisible by 3 */
        while (next_cand % 3 == 0) next_cand += 2;
        if (has_root6(next_cand))
            push_term((seq_t)next_cand);
        next_cand += 2;                 /* next odd candidate */
    }
}

/* ---------- public API ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset < 1 */
    ensure_terms_up_to(n);
    return term_arr[n - 1];
}
