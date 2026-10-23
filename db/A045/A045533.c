/*
 * A045533  Concatenate the n-th and (n+1)st prime.
 * https://oeis.org/A045533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat(prime(n), prime(n+1))
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 4489, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation keeps an internal cache of the prime numbers
 * needed for the requested n.  It generates primes on demand using
 * a simple trial division against previously found primes.  The cache
 * is expanded only when a larger n is requested, so repeated calls
 * do not recompute earlier primes.  The concatenation is formed by
 * multiplying the first prime by 10^d, where d is the number of
 * decimal digits of the second prime, and then adding the second prime.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stddef.h>   /* size_t */

/* ---- Internal prime cache --------------------------------------- */

/* Prime numbers are stored as uint64_t.  The cache grows geometrically. */
static uint64_t *prime_cache     = NULL;   /* array of primes */
static size_t   prime_cache_n    = 0;       /* number of primes currently stored */
static size_t   prime_cache_cap  = 0;       /* allocated capacity */
static uint64_t next_candidate   = 2;       /* next number to test for primality */

/* Allocate or enlarge the cache to at least the requested capacity. */
static void prime_cache_grow_to (size_t need_cap)
{
    if (prime_cache_cap >= need_cap)
        return;
    size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 128;
    if (new_cap < need_cap)
        new_cap = need_cap;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp)             /* allocation failed – abort */
        abort();
    prime_cache     = tmp;
    prime_cache_cap = new_cap;
}

/* Integer square root by incremental search – good enough for numbers
 * up to a few million (for 10000th prime this is adequate). */
static inline uint64_t isqrt_u64 (uint64_t x)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

/* Test whether x is prime by checking divisibility by all previously
 * stored primes less than or equal to sqrt(x).  Uses only operations
 * allowed for arbitrary‑precision types. */
static inline int is_candidate_prime (uint64_t x)
{
    if (x < 2)
        return 0;
    uint64_t lim = isqrt_u64(x);
    for (size_t i = 0; i < prime_cache_n && prime_cache[i] <= lim; ++i) {
        if (x % prime_cache[i] == 0)
            return 0;
    }
    return 1;
}

/* Ensure that the cache contains at least `cnt` primes (1‑based).  */
static void ensure_primes (size_t cnt)
{
    if (prime_cache_n >= cnt)
        return;

    prime_cache_grow_to(cnt);

    while (prime_cache_n < cnt) {
        while (!is_candidate_prime(next_candidate))
            ++next_candidate;      /* skip non‑primes */
        prime_cache[prime_cache_n++] = next_candidate;
        ++next_candidate;
    }
}

/* ---- Helper: number of decimal digits of a uint64_t ---------------- */
static inline int digit_count_u64 (uint64_t n)
{
    int d = 0;
    do {
        ++d;
        n /= 10;
    } while (n);
    return d;
}

/* ---- Power of ten (returns a seq_t value) -------------------------- */
static inline seq_t pow10_seq (int exponent)
{
    seq_t r = 1;
    while (exponent-- > 0)
        r *= (seq_t)10;
    return r;
}

/* ---- Sequence function -------------------------------------------- */
seq_t a (idx_t n)
{
    /*  Off‑by‑one: n is 1‑based per OEIS.  Undefined for n<1.  */
    if (n < (idx_t)1)
        return (seq_t)0;

    /*  Need at least (n+1)-th prime (1‑based).  */
    ensure_primes((size_t)n + 1);

    /*  Prime indices in the cache are 0‑based.  */
    uint64_t p1 = prime_cache[(size_t)n - 1]; /* nth prime */
    uint64_t p2 = prime_cache[(size_t)n];     /* (n+1)‑st prime */

    /*  Number of digits of p2.  */
    int d = digit_count_u64(p2);

    /*  Concatenation: p1 * 10^d + p2.  */
    seq_t pow   = pow10_seq(d);
    seq_t result = (seq_t)p1 * pow + (seq_t)p2;

    return result;
}
