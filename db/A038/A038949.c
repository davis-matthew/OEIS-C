/*
 * A038949  Primes p such that 67 is a square mod p.
 * https://oeis.org/A038949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 982 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the n‑th term of OEIS sequence A038949: primes p such that
 * 67 is a quadratic residue modulo p (i.e., Legendre(67|p)=1 or p=67).
 * The function caches qualifying primes and returns the n‑th one
 * (1‑based indexing as in the OEIS entry).  It works in any call order
 * and is efficient for values up to at least n=10000.
 */

#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
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

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *prime_cache = NULL;  /* dynamically sized array of qualified primes */
static size_t cache_len  = 0;       /* number of primes currently stored */
static size_t cache_cap  = 0;       /* allocated capacity of prime_cache */

static uint64_t next_candidate = 3;  /* next odd number to test for primality */

/* Adjust the cache capacity to hold at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;

    seq_t *p = seq_realloc(prime_cache, cache_cap, new_cap);
    prime_cache = p;
    cache_cap   = new_cap;
}

/* Return the n-th term (1‑based).  n==0 is undefined; return 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Extend cache until it contains at least n qualifying primes. */
    while (cache_len < (size_t)n) {
        uint64_t p = next_candidate;
        next_candidate += 2;  /* advance to next odd candidate */

        /* Skip composite numbers. */
        if (!is_prime_big((seq_t)p))
            continue;

        bool ok = false;
        if (p == 67ULL) {
            ok = true;                  /* Legendre(67|67)=0, treated as square */
        } else {
            int ls = jacobi_symbol(67ULL, p); /* Legendre(67|p) */
            if (ls == 1) ok = true;
        }

        if (ok) {
            grow_cache(cache_len + 1);
            prime_cache[cache_len++] = (seq_t)p;
        }
    }

    return prime_cache[n - 1];
}
