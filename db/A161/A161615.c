/*
 * A161615  Primes p such that 2*p+3234846615 is also a prime number.
 * https://oeis.org/A161615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 68 terms through n = 1000 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* Sequence A161615: primes p such that 2*p + 3234846615 is also prime. */
/* The function builds the sequence on demand, caching results for
   efficiency.  It works for every supported build (C or C++), including
   arbitrary‑precision seq_t values. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
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

#ifdef __cplusplus
#include <new>   /* for operator new[] / delete[] */
#endif

/* Helper functions that work for both C and C++ */
static seq_t* alloc_seq_buf(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t*)malloc(n * sizeof(seq_t));
#endif
}

static void free_seq_buf(seq_t* p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Cache state */
static seq_t *terms      = NULL;  /* stored primes p            */
static size_t terms_len  = 0;      /* how many terms we have    */
static size_t terms_cap  = 0;      /* capacity of the cache      */
static uint64_t last_prime = 0;    /* last prime examined        */

#define GROW_CAP 32

static void ensure_capacity(size_t needed)
{
    if (needed <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap : GROW_CAP;
    while (new_cap < needed)
        new_cap += GROW_CAP;

    seq_t *new_terms = alloc_seq_buf(new_cap);
    if (!new_terms) {
        /* fatal error – abort */
#ifdef __cplusplus
        std::abort();
#else
        abort();
#endif
    }
    for (size_t i = 0; i < terms_len; ++i)
        new_terms[i] = terms[i];
    free_seq_buf(terms);
    terms      = new_terms;
    terms_cap  = new_cap;
}

static void extend_to_n(idx_t n)
{
    if ((idx_t)terms_len >= n)
        return;

    const seq_t C = (seq_t)3234846615ULL;   /* 3*5*7*11*13*17*19*23*29 */

    /* Start searching from the prime after the last examined value. */
    uint64_t cand = (last_prime == 0) ? 2 : last_prime + 1;

    while ((idx_t)terms_len < n) {
        /* Find the next prime >= cand. */
        cand = next_prime(cand);
        /* Compute 2*p + C.  For 64‑bit or smaller seq_t this will fit;
           for arbitrary‑precision seq_t use its constructors. */
        seq_t val = (seq_t)cand * (seq_t)2 + C;
        if (is_prime_big(val)) {
            ensure_capacity(terms_len + 1);
            terms[terms_len++] = (seq_t)cand;
        }
        /* Advance past the prime just used. */
        last_prime = cand;
        ++cand;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* undefined – only positive indices are valid */
    extend_to_n(n);
    return terms[n - 1];
}
