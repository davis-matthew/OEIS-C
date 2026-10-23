/*
 * A140618  Primes of the form 20*x^2+4*x*y+23*y^2.
 * https://oeis.org/A140618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 405, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS entry #140618 : Primes of the form 20*x^2+4*x*y+23*y^2
   Formula: a(n) = the nth prime that can be expressed as 20*x^2+4*x*y+23*y^2
   (index offset = 1)

   The function a(idx_t n) returns the n-th term (1‑based). It uses a small
   sieve to generate all primes up to 30 million and tests each prime for
   representability by the given quadratic form.  Only 64‑bit arithmetic is
   required for the test; the result is converted into seq_t for storage.
   The algorithm is fully deterministic, variable‑order safe and runs well
   under two seconds for n up to ~10 000. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"

#define SIEVE_LIMIT 30000000u  /* primes up to this bound are enough for n≈10⁴ */

/* Simple square‑root for 64‑bit unsigned integers */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t r = 0, bit = 1ULL << 62;
    while (bit > n) bit >>= 2;
    while (bit) {
        uint64_t y = r + bit;
        if (n >= y) {
            n -= y;
            r = (r >> 1) + bit;
        } else
            r >>= 1;
        bit >>= 2;
    }
    return r;
}

/* Prime sieve – produces all primes ≤ SIEVE_LIMIT.
   The arrays are static because calls may be out of order; once built
   the same list is reused. */
static uint32_t *primes      = NULL;
static size_t   prime_count  = 0;
static bool     sieve_done   = false;

static void init_primes(void)
{
    if (sieve_done) return;

    uint8_t *is_comp = (uint8_t *)calloc((size_t)SIEVE_LIMIT + 1, 1);
    if (!is_comp) abort();

    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i)
        if (!is_comp[i])
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_comp[j] = 1;

    /* count primes */
    prime_count = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_comp[i]) ++prime_count;

    primes = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!primes) abort();

    size_t pos = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_comp[i])
            primes[pos++] = i;

    free(is_comp);
    sieve_done = true;
}

/* Check whether prime p can be represented as 20x^2+4xy+23y^2.
   The test uses the discriminant -1824; for primes not dividing the
   discriminant it is necessary but not sufficient, so a quadratic search
   over y is performed.  Only y≥0 are needed because (x,y) and (-x,-y)
   give the same value. */
static inline bool representable_prime(uint32_t p)
{
    /* trivial checks */
    if (p < 23) return false;          /* smallest representable prime */
    if (p == 2 || p == 3 || p == 19) return false; /* divisors of the discr. */

    /* Require that the Legendre/Jacobi symbol ( -1824 / p ) = +1. */
    int jac = jacobi_symbol(1824ull, p);    /* (1824/p) */
    if (jac == 0) return false;
    int sign = (p % 4 == 1) ? 1 : -1;      /* (-1/p) */
    if ((jac * sign) != 1) return false;

    /* Compute the bound for |y| such that 80*p - 1824*y^2 >= 0.
       That is y^2 <= (80*p)/1824.  The value fits in 64‑bit. */
    __int128 temp   = (__int128)80 * p;
    uint64_t limit  = (uint64_t)(temp / 1824);
    uint64_t yLimit = isqrt_u64(limit);

    for (uint64_t y = 0; y <= yLimit; ++y) {
        uint64_t lhs = 80ULL * p - 1824ULL * y * y;          /* Δ */
        uint64_t k   = isqrt_u64(lhs);
        if (k * k != lhs) continue;                         /* Δ not square */

        uint64_t mod4y = (4ULL * y) % 40;
        uint64_t r     = k % 40;
        if (r == mod4y || r == (40 - mod4y) % 40)
            return true;
    }
    return false;
}

/* Sequence cache – stores the values found so far.
   cache_len is the number of terms stored. */
static seq_t *cache    = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Current position in the prime list (index into primes[]). */
static size_t next_prime_idx = 0;

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined outside the offset */

    init_primes();

    /* Grow the cache if necessary. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms until we have a(n). */
    while (cache_len < (size_t)n) {
        /* Scan primes searching for the next representable one. */
        while (next_prime_idx < prime_count) {
            uint32_t p = primes[next_prime_idx++];
            if (representable_prime(p)) {
                cache[++cache_len] = (seq_t)p;   /* convert to seq_t */
                break;
            }
        }
        /* The sieve limit is sufficient for n up to about 10 000. */
    }

    return cache[(size_t)n];
}
