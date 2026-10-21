/*
 * A071956  Table in which n-th row list prime factors and their exponents in
 *          factorization of prime(n)!.
 * https://oeis.org/A071956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) enumerates the flattened table of prime factors and
 * exponents of prime(n)!.
 *
 * Let p_r = prime(r).  The r‑th row of the table lists the prime factors
 * of p_r! together with their exponents:
 *
 *   p_1  e_1  p_2  e_2  ...  p_k  e_k
 *
 * where p_i is the i‑th prime (p_1 = 2, p_2 = 3, ...) and
 * e_i = v_{p_i}(p_r!) = ⌊p_r/p_i⌋ + ⌊p_r/p_i²⌋ + … .
 *
 * The global sequence a(n) is produced by writing all rows one after
 * another.  This implementation finds the row and the position within
 * the row for any given index n (1‑based) and returns the appropriate
 * prime number or exponent.
 *
 * Complexity:  O(log p_r) for one term, where p_r is the r‑th prime;
 * for n up to ~10^4 the r‑th prime is < 1.1×10^6, so the routine is
 * easily fast enough.  No hard‑coded table is used; primes are generated
 * once with a small sieve (up to 200 k), which is more than enough for
 * the required range.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* helper: floor square root of a 64‑bit unsigned integer */
static unsigned long long floor_sqrt_u64(unsigned long long x) {
    unsigned long long lo = 0, hi = (x >> 1) + 1;           /* hi >= sqrt(x) */
    while (lo < hi) {
        unsigned long long mid = (lo + hi + 1) >> 1;         /* upper mid */
        if (mid <= x / mid) lo = mid;
        else                hi = mid - 1;
    }
    return lo;
}

/* helper: exponent of prime q in t! (Legendre's formula) */
static seq_t legendre_exp(idx_t t, idx_t q) {
    seq_t exp = 0;
    idx_t div = q;
    while (div <= t) {
        exp += (seq_t)(t / div);
        if (div > t / q) break;       /* avoid overflow */
        div *= q;
    }
    return exp;
}

/* static storage for primes and initialization guard */
static bool _primed = false;
static uint32_t * _primes = NULL;          /* list of primes */
static size_t    _prime_count = 0;         /* number of primes stored */

/* pre‑sieve primes up to a fixed limit (enough for n≈10 000) */
static void _build_primes(void) {
    const size_t LIMIT = 200000;            /* > prime(10000) */
    bool *is_prime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    /* count primes and allocate array */
    _prime_count = 0;
    for (size_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++_prime_count;

    _primes = (uint32_t*)malloc(_prime_count * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) _primes[idx++] = (uint32_t)i;

    free(is_prime);
}

/* main sequence function */
seq_t a(idx_t n) {
    if (!_primed) {                        /* one‑time initialisation */
        _build_primes();
        _primed = true;
    }

    if (n == 0) return 0;                  /* safety for non‑positive n */

    /* find the row r such that r*(r+1) >= n */
    unsigned long long val = 4ULL * n + 1;   /* 1 + 4n */
    unsigned long long s = floor_sqrt_u64(val);   /* floor sqrt(1+4n) */
    unsigned long long r = (s - 1) / 2;             /* floor value */
    if (r * (r + 1) < n) r++;                     /* correct to ceil */

    /* position inside the row (0‑based) */
    unsigned long long prev = (r - 1) * r;       /* terms before this row */
    unsigned long long offset = n - prev - 1;    /* 0‑based index in row */

    unsigned long long prime_idx = offset / 2;   /* index of prime factor */
    uint32_t prime_val = _primes[prime_idx];    /* the prime itself */

    if (offset % 2 == 0) {
        /* even offset → prime factor itself */
        return (seq_t)prime_val;
    } else {
        /* odd offset → exponent of that prime in factorial of p_r */
        idx_t factorial_prime = _primes[r - 1];   /* p_r */
        seq_t e = legendre_exp(factorial_prime, prime_val);
        return e;
    }
}
