/*
 * A077449  Numbers k such that Sum_{d|k} mu(phi(d)) = 0.
 * https://oeis.org/A077449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   add_divisor_sums
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077449
 *
 * This routine returns the n‑th integer k such that
 *   Sum_{d|k} μ(φ(d)) = 0
 * The sequence is indexed starting at 1:  a(1)=3, a(2)=6, …
 * The implementation precomputes φ and μ up to an upper bound and
 * evaluates the divisor sum for each integer until the requested
 * index is reached.  It keeps the results cached so that repeated
 * calls with arbitrary ordering of n are fast.
 *
 * Author: ChatGPT
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/add_divisor_sums.h"

/* forward declaration: seq_t and idx_t are defined by the build */
extern unsigned long long dummy; /* to keep the compiler quiet */

/* Helper macros for casting to the generic types */
#define TO_SEQ(x) ((seq_t)(x))
#define TO_IDX(x) ((idx_t)(x))

/* Static state */
static idx_t   maxN           = 2000000;          /* maximum number precomputed (adjustable) */
static int    *phi            = NULL;             /* φ(n) for 1..maxN */
static int    *mu             = NULL;             /* μ(n) for 1..maxN */
static int    *mu_phi         = NULL;             /* μ(φ(n)) for 1..maxN */
static int    *spf            = NULL;             /* smallest prime factor */

static idx_t   last_k         = 0;                /* last integer checked */
static idx_t   seq_len        = 0;                /* number of collected terms */
static seq_t *seq_cache       = NULL;             /* array of collected terms */
static size_t seq_cap        = 0;                /* capacity of seq_cache */

/* ---------------------  Utility  -------------------------------- */

/* Linear sieve to fill spf, phi, mu, and mu_phi */
static void init_sieve(void)
{
    if (phi != NULL) return;   /* already initialized */

    phi   = (int*)calloc(maxN + 1, sizeof(int));
    mu    = (int*)calloc(maxN + 1, sizeof(int));
    mu_phi= (int*)calloc(maxN + 1, sizeof(int));
    spf   = (int*)calloc(maxN + 1, sizeof(int));
    if (!phi || !mu || !mu_phi || !spf) abort();

    for (idx_t i = 0; i <= maxN; ++i) spf[i] = (int)i;
    phi[1] = 1;
    mu[1]  = 1;

    for (idx_t i = 2; i <= maxN; ++i)
    {
        if (spf[i] == (int)i)          /* i is prime */
        {
            for (idx_t j = i; j <= maxN; j += i)
                if (spf[j] == (int)j) spf[j] = (int)i;
        }
    }

    for (idx_t i = 2; i <= maxN; ++i)
    {
        int n = (int)i;
        int lastp = 0;
        int res = (int)i;
        bool squarefree = true;
        int sign = 1;
        while (n > 1)
        {
            int p = spf[n];
            int cnt = 0;
            while (n % p == 0) { n /= p; ++cnt; }
            if (cnt > 1) { squarefree = false; }
            if (lastp == p) { squarefree = false; }
            if (!squarefree) { sign = 0; }
            else {
                sign = -sign;
            }
            res -= res / p;
            lastp = p;
        }
        phi[i] = res;
        mu[i]  = sign;
    }

    for (idx_t i = 1; i <= maxN; ++i)
        mu_phi[i] = mu[(int)phi[i]];
}

static void recurse_divisors(int idx, int m, int cur, int *p, int *e, int *sum)
{
    if (idx == m) {
        *sum += mu_phi[cur];
        return;
    }
    int val = cur;
    for (int exp = 0; exp <= e[idx]; ++exp) {
        recurse_divisors(idx + 1, m, val, p, e, sum);
        val *= p[idx];
    }
}

/* Test if the number k satisfies the required sum==0.     */
static bool satisfies(idx_t k)
{
    int primes[16];
    int exps[16];
    int m = 0;
    int temp = (int)k;
    while (temp > 1) {
        int p = spf[temp];
        int cnt = 0;
        while (temp % p == 0) { temp /= p; ++cnt; }
        primes[m] = p;
        exps[m]   = cnt;
        ++m;
    }
    int sum = 0;
    recurse_divisors(0, m, 1, primes, exps, &sum);
    return sum == 0;
}

/* Ensure we have at least 'n' terms in the cache */
static void ensure_terms(idx_t n)
{
    if (seq_len >= n) return;

    /* Extend cache if necessary */
    if (seq_cap < n) {
        size_t newcap = seq_cap? seq_cap*2: 128;
        if (newcap < n) newcap = n;
        seq_cache = (seq_t*)realloc(seq_cache, newcap * sizeof(seq_t));
        seq_cap = newcap;
    }

    /* Compute terms until we reach n */
    while (seq_len < n) {
        ++last_k;
        if (last_k > maxN) {
            /* Not enough precomputed range – extend arrays (rare) */
            /* For simplicity, abort; in real use you might recompute larger maxN. */
            abort();
        }
        if (satisfies(last_k)) {
            seq_cache[seq_len++] = TO_SEQ(last_k);
        }
    }
}

/* Public API */
seq_t a(idx_t n)
{
    init_sieve();
    ensure_terms(n);
    return seq_cache[n - 1];
}
