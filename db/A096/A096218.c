/*
 * A096218  Primes of the form 3*prime(n) + 2*prime(n+1).
 * https://oeis.org/A096218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*p(k) + 2*p(k+1), where p(k) is the k-th prime. The
 *            function returns the n-th term (n ≥ 1)
 * Range:     u8 n <= 12, u16 n <= 466, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 986 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Static state shared between calls */
static seq_t *terms = NULL;            /* stored sequence values */
static size_t term_count = 0;          /* number of terms computed so far */
static size_t terms_cap = 0;           /* allocated capacity for terms */
static uint64_t p1 = 0;                /* current prime p(k)   */
static uint64_t p2 = 0;                /* next prime p(k+1)   */
static bool initialized = false;      /* whether primes are seeded */

/* Ensure enough capacity for the terms array */
static void grow_terms(size_t needed)
{
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, terms, new_cap);
    if (!tmp) exit(EXIT_FAILURE); /* out of memory */
    terms = tmp;
    terms_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for 0 */

    /* Seed the first two primes on first call */
    if (!initialized) {
        p1 = 2;
        p2 = next_prime(p1);   /* next prime > 2  */
        initialized = true;
    }

    /* Generate terms until we have n of them */
    while (term_count < (size_t)n) {
        /* Compute candidate value 3*p1 + 2*p2 as seq_t */
        seq_t val = (seq_t)3 * p1 + (seq_t)2 * p2;

        /* If the candidate itself is prime, store it */
        if (is_prime_big(val)) {
            grow_terms(term_count + 1);
            terms[term_count++] = val;
        }

        /* Advance to the next consecutive prime pair */
        p1 = p2;
        p2 = next_prime(p1);
    }

    return terms[(size_t)n - 1];
}
