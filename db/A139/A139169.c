/*
 * A139169  a(n)=smallest k >= 1 such that n divides prime(k)!.
 * https://oeis.org/A139169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 1 such that n divides prime(k)!
 * Range:     u8 n <= 1618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses Legendre’s formula to find, for each prime
   divisor p^e of n, the minimal m such that p^e divides m!.
   The largest such m over all prime divisors is the smallest integer
   m that makes prime(k)! divisible by n for some k, hence the required
   k is the index (1‑based) of the first prime ≥ m.                 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cache of primes discovered so far.
   The cache holds all primes up to the current maximum needed.      */
static uint64_t *prime_cache = NULL;   /* array of primes            */
static size_t    prime_cached = 0;     /* number of primes stored    */
static uint64_t  prime_last   = 1;     /* last prime in the cache    */

/* Legendre’s formula: exponent of prime p in m! */
static idx_t exp_in_factorial(idx_t m, uint64_t p) {
    idx_t exp = 0;
    idx_t div = p;
    while (div <= m) {
        exp += m / div;
        if (div > m / p) break;          /* prevent overflow in div *= p */
        div *= p;
    }
    return exp;
}

/* Minimal m such that exponent of p in m! is at least e.          */
static idx_t minimal_m_for_prime(uint64_t p, int e) {
    idx_t low  = p;                     /* at least p for exponent 1   */
    idx_t high = p * (idx_t)e;          /* looser upper bound          */
    if (high < low) high = UINT64_MAX;  /* safeguard for overflow      */
    idx_t ans = high;

    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (exp_in_factorial(mid, p) >= e) {
            ans = mid;
            if (mid == 0) break;        /* avoid infinite loop */
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return ans;
}

/* Expand the prime cache until its last prime is ≥ limit.         */
static void ensure_primes_ge(idx_t limit) {
    if (prime_cached == 0) {
        /* initialise with a single prime: 2 */
        prime_cache = (uint64_t*)malloc(sizeof(uint64_t));
        if (!prime_cache) exit(1);
        prime_cache[0] = 2;
        prime_cached  = 1;
        prime_last    = 2;
    }
    if (prime_last >= limit) return;

    /* start searching from the next integer after the current last prime */
    uint64_t candidate = prime_last + 1;
    if (candidate <= 2) candidate = 2;
    if (candidate % 2 == 0) candidate++;          /* make odd           */

    while (prime_last < limit) {
        /* simple trial division using the cache itself              */
        bool is_prime = true;
        for (size_t i = 0; i < prime_cached; ++i) {
            uint64_t q = prime_cache[i];
            if (q * q > candidate) break;
            if (candidate % q == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            prime_cache = (uint64_t*)realloc(prime_cache,
                                             (prime_cached + 1) * sizeof(uint64_t));
            if (!prime_cache) exit(1);
            prime_cache[prime_cached++] = candidate;
            prime_last = candidate;
        }
        candidate += 2;   /* skip even numbers                         */
    }
}

/* Binary search over the prime cache to find the first index k such that
   prime_cache[k] ≥ value.  The index is 0‑based.                     */
static size_t lower_bound_prime_cache(idx_t value) {
    size_t left = 0, right = prime_cached;
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (prime_cache[mid] >= value) right = mid;
        else left = mid + 1;
    }
    return left;   /* may equal prime_cached; in that case the loop      */
}

/* Main sequence function                                              */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;          /* a(1) = 1 by definition             */

    /* ---------------- factorise n ---------------------------------- */
    uint64_t m = (uint64_t)n;             /* working copy of n                  */
    uint64_t primes_factor[16];
    int       exponents[16];
    size_t    factor_cnt = 0;
    uint64_t p;

    /* trial division up to sqrt(m) */
    for (p = 2; p * p <= m; /* increment inside */) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            primes_factor[factor_cnt] = p;
            exponents[factor_cnt++]   = e;
        }
        p = (p == 2) ? 3 : p + 2;   /* 2 first, then only odds            */
    }
    if (m > 1) {                    /* remaining prime factor               */
        primes_factor[factor_cnt] = m;
        exponents[factor_cnt++]   = 1;
    }

    /* ---------------- find required m ------------------------------ */
    idx_t required_m = 0;
    for (size_t i = 0; i < factor_cnt; ++i) {
        idx_t mp = minimal_m_for_prime(primes_factor[i], exponents[i]);
        if (mp > required_m) required_m = mp;
    }

    /* ---------------- find the smallest prime ≥ required_m --------- */
    ensure_primes_ge(required_m);
    size_t idx = lower_bound_prime_cache(required_m); /* 0‑based index        */

    /* index in OEIS starts at 1, so add 1                          */
    return (seq_t)(idx + 1);
}
