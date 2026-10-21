/*
 * A066697  Numbers k such that Euler phi(k) / Carmichael lambda(k) = 18.
 * https://oeis.org/A066697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 339, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence of integers k such that Euler's totient
   φ(k) divided by the Carmichael λ(k) equals 18. */
#include <stdint.h>
#include <stddef.h>

/* type aliases supplied by the build */
typedef seq_t a_value_t;
typedef idx_t a_index_t;

static const a_index_t MAX_LIMIT = 20000000;          /* safe upper bound */
static uint32_t *spf = NULL;                         /* smallest prime factor */
static int *primes = NULL;
static size_t prime_cnt = 0;
static size_t spf_sq = 0;                            /* limit of computed spf */
static a_value_t *terms = NULL;
static size_t terms_cnt = 0;
static a_index_t last_enum = 1;                      /* last number processed */

/* simple Euclidean gcd for unsigned integers */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* compute φ(n) using the factorisation given by spf */
static a_value_t phi_uint64(uint64_t n)
{
    a_value_t result = n;
    uint64_t m = n;
    while (m > 1) {
        uint32_t p = spf[m];
        result = result / p * (p - 1);
        while (m % p == 0) m /= p;
    }
    return result;
}

/* compute λ(n) using the factorisation given by spf */
static a_value_t lambda_uint64(uint64_t n)
{
    uint64_t l = 1;
    uint64_t m = n;
    while (m > 1) {
        uint32_t p = spf[m];
        int exp = 0;
        while (m % p == 0) { m /= p; ++exp; }
        uint64_t lam;
        if (p == 2) {
            if (exp == 1)      lam = 1;           /* λ(2)   = 1   */
            else if (exp == 2) lam = 2;           /* λ(4)   = 2   */
            else                lam = 1ULL << (exp - 2); /* λ(2^a)=2^(a-2) */
        } else {
            lam = 1;
            for (int i = 1; i < exp; ++i) lam *= p; /* p^(a-1) */
            lam *= (p - 1);                       /* * (p-1) */
        }
        uint64_t g = gcd_u64(l, lam);
        l = (l / g) * lam; /* lcm step */
    }
    return (a_value_t)l;
}

/* initialise the linear sieve up to MAX_LIMIT */
static void init_sieve(void)
{
    spf = (uint32_t*)malloc((MAX_LIMIT + 1) * sizeof(uint32_t));
    primes = (int*)malloc((MAX_LIMIT / 10) * sizeof(int));
    for (size_t i = 0; i <= MAX_LIMIT; ++i) spf[i] = 0;
    prime_cnt = 0;
    for (size_t i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == 0) { spf[i] = (uint32_t)i; primes[prime_cnt++] = (int)i; }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint64_t p = primes[j];
            if (p > spf[i] || i * p > MAX_LIMIT) break;
            spf[i * p] = (uint32_t)p;
        }
    }
    spf_sq = MAX_LIMIT;
}

/* generate terms until we have at least n terms */
static void ensure_terms(a_index_t n_needed)
{
    while (terms_cnt < n_needed) {
        if (last_enum > MAX_LIMIT) break;   /* should not happen for N <= 1e4 */
        ++last_enum;
        /* compute phi and lambda */
        a_value_t phi = phi_uint64(last_enum);
        a_value_t lam = lambda_uint64(last_enum);
        if (phi == 18ULL * lam) {
            terms = (a_value_t*)realloc(terms, sizeof(a_value_t) * (terms_cnt + 1));
            terms[terms_cnt++] = (a_value_t)last_enum;
        }
    }
}

/* main sequence function */
a_value_t a(a_index_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_sieve();
        terms = NULL;
        terms_cnt = 0;
        last_enum = 1;
        initialized = 1;
    }
    if (n == 0) return 0;     /* undefined for n=0, return 0 just in case */
    ensure_terms(n);
    return terms[n - 1];
}
