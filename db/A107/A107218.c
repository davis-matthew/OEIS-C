/*
 * A107218  Primes of the form 4x^2 + 25y^2.
 * https://oeis.org/A107218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime p that can be written as p = 4*x^2 +
 *            25*y^2 for integers x,y ≥ 0. The sequence starts 29, 41, 61, 89,
 *            …
 * Range:     u8 n <= 6, u16 n <= 822, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* dynamically grown array of primes (uint64_t) */
static size_t primes_cap = 0;
static size_t primes_sz  = 0;
static uint64_t *primes  = NULL;

/* array of terms (uint64_t): the primes that satisfy the form */
static size_t terms_cap   = 0;
static size_t terms_sz    = 0;
static uint64_t *terms    = NULL;

/* the next integer to test */
static uint64_t current = 2;

/* ------------------------------------------------------------------ */
/* reallocate the primes array so it can hold at least 'cap' elements */
static void ensure_primes_cap(size_t cap)
{
    if (cap <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap << 1 : 64;
    while (new_cap < cap) new_cap <<= 1;
    uint64_t *newp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!newp) exit(1);
    primes = newp;
    primes_cap = new_cap;
}

/* reallocate the terms array so it can hold at least 'cap' elements */
static void ensure_terms_cap(size_t cap)
{
    if (cap <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 64;
    while (new_cap < cap) new_cap <<= 1;
    uint64_t *newp = (uint64_t *)realloc(terms, new_cap * sizeof(uint64_t));
    if (!newp) exit(1);
    terms = newp;
    terms_cap = new_cap;
}

/* add a prime to the primes list */
static void add_prime(uint64_t p)
{
    ensure_primes_cap(primes_sz + 1);
    primes[primes_sz++] = p;
}

/* add a term to the terms list */
static void add_term(uint64_t v)
{
    ensure_terms_cap(terms_sz + 1);
    terms[terms_sz++] = v;
}

/* integer square root (floor) of 64‑bit number */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t low = 0, high = ((uint64_t)1 << 32);
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid > n / mid) high = mid - 1;
        else low = mid;
    }
    return low;
}

/* test if n is a perfect square */
static inline bool is_square_uint64(uint64_t n)
{
    uint64_t r = isqrt_uint64(n);
    return r * r == n;
}

/* simple primality test using the accumulated list of primes */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_sz; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* check whether a prime p can be written as 4x² + 25y² */
static inline bool has_representation(uint64_t p)
{
    if (p < 29) return false;                /* smallest such prime is 29 */
    uint64_t ymax = isqrt_uint64(p / 25ULL);
    for (uint64_t y = 1; y <= ymax; ++y) {
        uint64_t rem = p - 25ULL * y * y;
        if (rem & 3ULL) continue;            /* rem not divisible by 4 */
        uint64_t x2 = rem >> 2;              /* x² = rem/4 */
        if (is_square_uint64(x2)) return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* required function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1, but harmless */

    /* generate terms until we have at least n of them */
    while (terms_sz < (size_t)n) {
        if (is_prime_uint64(current)) {
            if (has_representation(current))
                add_term(current);
            add_prime(current);
        }
        ++current;
    }
    return (seq_t)terms[n - 1];
}
