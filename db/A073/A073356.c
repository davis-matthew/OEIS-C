/*
 * A073356  Greatest common divisor of squarefree kernel of n and sum of
 *          squarefree kernels of numbers <= n.
 * https://oeis.org/A073356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( A007947(n), A073355(n) )
 * Range:     u8 n <= 29, u16 n <= 847, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A007947(n) = product of distinct prime factors of n (square‑free kernel) */
/* A073355(n) = sum_{k=1..n} A007947(k) (cumulative sum of square‑free kernels) */

#include <stddef.h>
#include <stdlib.h>

#if defined(__cplusplus)
#include <vector>
#endif

/* ----- static state for incremental computation ----- */
static idx_t max_n = 0;   

#if defined(__cplusplus)
/* use vector in C++ to properly construct/dispose arbitrary‑precision types */
static std::vector<idx_t> spf;
static std::vector<idx_t> primes;
static std::vector<seq_t> rad;
static std::vector<seq_t> pref;
#else
/* fall back to plain pointers in C */
static idx_t *spf = NULL;
static idx_t *primes = NULL;
static seq_t *rad = NULL;
static seq_t *pref = NULL;
#endif

/* Euclidean GCD for any integral seq_t type */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Build sieve and compute rad and prefix sums up to N */
static void build_sieve(idx_t N)
{
    /* Grow storage if necessary */
#if defined(__cplusplus)
    const size_t sz = static_cast<size_t>(N) + 1;
    spf.resize(sz);
    primes.resize(sz);
    rad.resize(sz);
    pref.resize(sz);
#else
    const size_t sz = (size_t)N + 1;
    spf     = (idx_t *)realloc(spf,     sz * sizeof(idx_t));
    primes  = (idx_t *)realloc(primes,  sz * sizeof(idx_t));
    rad     = (seq_t *)realloc(rad,     sz * sizeof(seq_t));
    pref    = (seq_t *)realloc(pref,    sz * sizeof(seq_t));
#endif

    /* initialize spf array */
#if defined(__cplusplus)
    spf[0] = 0;
    spf[1] = 1;
    for (idx_t i = 2; i <= (idx_t)N; ++i) spf[i] = 0;
#else
    spf[0] = 0;
    spf[1] = 1;
    for (idx_t i = 2; i <= N; ++i) spf[i] = 0;
#endif

    /* Linear sieve to compute spf up to N */
    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[primes_cnt++] = i;
        }
        for (size_t k = 0; k < primes_cnt; ++k) {
            idx_t p = primes[k];
            if (p > spf[i] || i * p > N) break;
            spf[i * p] = p;
        }
    }

    /* Compute rad and prefix sums */
    rad[1]  = 1;
    pref[1] = rad[1];
    for (idx_t i = 2; i <= N; ++i) {
        idx_t p = spf[i];
        idx_t m = i / p;
        seq_t r = rad[m];
        if (m % p != 0) r *= p;   /* p occurs the first time */
        rad[i]  = r;
        pref[i] = pref[i - 1] + r;
    }

    max_n = N;
}

/* Public entry point for sequence A073356 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0, safety guard */

    if (n > max_n) build_sieve(n);
    return seq_gcd(rad[n], pref[n]);   /* a(n) = gcd(rad(n), Σ_{k≤n} rad(k)) */
}
