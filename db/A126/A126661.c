/*
 * A126661  Array read by antidiagonals: A(i,j) = smallest odd prime r such
 *          that pqr+2 is prime, where p is the i-th odd prime and q is the
 *          j-th odd prime.
 * https://oeis.org/A126661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, tabl, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 99 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A126661: a(n) = smallest odd prime r such that oddprime(k)*oddprime(l)*r+2 is prime
   where (k,l) are the antidiagonal coordinates of n.  Compute on demand. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

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

/* Determine the antidiagonal coordinates (i,j) for a given 1‑based offset n. */
static void antidiag_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* Find smallest d with d*(d+1)/2 >= n */
    idx_t d = 1;
    while (d * (d + 1ULL) / 2ULL < n)
        ++d;

    idx_t prev = d * (d - 1ULL) / 2ULL;          /* terms before this diagonal */
    idx_t offset = n - prev;                    /* 1‑based offset on the diagonal */

    *i = offset;                /* 1‑based index for oddprime(i) */
    *j = d - offset + 1ULL;     /* 1‑based index for oddprime(j) */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1 */

    idx_t i, j;
    antidiag_coords(n, &i, &j);

    /* oddprime(k) = get_prime(k+1) because get_prime(1)=2, we need the odd primes */
    seq_t pi = get_prime(i + 1ULL); /* oddprime(i) */
    seq_t pj = get_prime(j + 1ULL); /* oddprime(j) */

    /* Search for the smallest odd prime r (starting from 3) with pi*pj*r+2 prime */
    for (idx_t idx = 0; ; ++idx) {
        seq_t r = get_prime(idx + 2ULL);           /* r = 3,5,7,... */
        seq_t val = pi * pj * r + 2ULL;
        if (is_prime_big(val))
            return r;
    }

    /* Unreachable */
    return 0;
}
