/*
 * A205386  Least k such that n divides s(k)-s(j) for some j<k, where
 *          s(j)=(1/2)C(2j,j).
 * https://oeis.org/A205386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Sequence A205386: 
 *  a(n) = least k such that n divides s(k)-s(j) for some j<k,
 *  where s(k) = C(2k,k)/2.
 *
 *  Computation: iterate k=1,2,... keeping track of residues of s(k) modulo n;
 *  the first time a residue repeats, return the current k.
 *
 *  s(k+1) = s(k) * 2(2k+1)/(k+1)  (exact division).
 *  We maintain the prime‐factorisation of s(k) to update s(k+1) efficiently
 *  and to compute s(k) mod n on the fly.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------ */
/*  CONSTANT AND PRIMES TABLE                               */
/* ------------------------------------------------------------ */
#define MAX_K 40000                /* upper bound for k needed for n≤10000 */
#define MAX_PRIMES 5000            /* enough for primes up to MAX_K */

static bool primes_inited = false;
static uint32_t spf[MAX_K + 1];     /* smallest prime factor */
static uint32_t primes[MAX_PRIMES];
static int       prime_count;
static int       prime_index[MAX_K + 1];

/* ------------------------------------------------------------ */
/*  INITIALISE PRIME TABLE (sieve of Eratosthenes)             */
/* ------------------------------------------------------------ */
static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;

    /* initialise spf array */
    for (int i = 0; i <= MAX_K; ++i) spf[i] = (uint32_t)i;
    for (int i = 2; i * i <= MAX_K; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (int j = i * i; j <= MAX_K; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }

    /* initialise prime_index array */
    for (int i = 0; i <= MAX_K; ++i) prime_index[i] = -1;
    prime_count = 0;
    for (int i = 2; i <= MAX_K; ++i) {
        if (spf[i] == (uint32_t)i) {           /* i is prime */
            if (prime_count >= MAX_PRIMES) break;
            primes[prime_count] = i;
            prime_index[i] = prime_count;
            ++prime_count;
        }
    }
}

/* ------------------------------------------------------------ */
/*  MODULAR POWER  (base^exp mod mod)                         */
/* ------------------------------------------------------------ */
static uint64_t pow_mod_uint64(uint64_t base, int exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return res;
}

/* ------------------------------------------------------------ */
/*  UPDATE EXPONENT ARRAY  (add or subtract prime exponents)  */
/* ------------------------------------------------------------ */
static void update_exponents(uint32_t x, int delta, int *exp)
{
    while (x > 1) {
        uint32_t p = spf[x];
        int cnt = 0;
        while (x % p == 0) { x /= p; ++cnt; }
        int idx = prime_index[p];
        exp[idx] += delta * cnt;
    }
}

/* ------------------------------------------------------------ */
/*  MAIN FUNCTION  (compute a(n))                              */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_primes();

    /* first time a residue appears */
    idx_t *first_seen = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!first_seen) return (seq_t)0;                /* unlikely */
    memset(first_seen, 0, (size_t)n * sizeof(idx_t));

    /* exponent array for the factorisation of current s(k) */
    int exp[MAX_PRIMES];
    memset(exp, 0, sizeof(exp));

    /* start with s(1) = 1  (no prime factors) */
    idx_t r = (n == 0) ? 0 : (idx_t)(1 % n);
    first_seen[r] = 1;                                 /* k = 1 */

    /* iterate k = 2,3,... until we see a repeated residue */
    for (idx_t k = 2; ; ++k) {
        /* factor 4k-2 (the numerator in the recurrence) */
        uint32_t mul = (uint32_t)(4 * k - 2);
        update_exponents(mul, +1, exp);

        /* factor k (the denominator) */
        uint32_t div = (uint32_t)k;
        update_exponents(div, -1, exp);

        /* compute s(k) modulo n from its prime factorisation */
        uint64_t mod = (uint64_t)n;
        uint64_t rr   = 1 % mod;
        for (int i = 0; i < prime_count; ++i) {
            int e = exp[i];
            if (e > 0)
                rr = (rr * pow_mod_uint64((uint64_t)primes[i], e, mod)) % mod;
        }
        r = (idx_t)rr;

        if (first_seen[r] != 0) {
            free(first_seen);
            return (seq_t)k;                    /* first repeat found */
        }
        first_seen[r] = k;
    }
}
