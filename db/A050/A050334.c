/*
 * A050334  Number of ordered factorizations of n into numbers with an odd
 *          number of prime divisors (prime factors counted with
 *          multiplicity).
 * https://oeis.org/A050334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n>1, a(n) = sum_{d|n , d>1 , omega(d) odd} a(n/d),
 *            where omega(d) counts prime factors of d with multiplicity
 * Range:     u8 n <= 719, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- global static state ---------- */
static seq_t *a_cache = NULL;      /* memoized a(n) values, 1‑based           */
static size_t a_cache_size = 0;    /* allocated size of a_cache              */
static idx_t  a_cache_max   = 0;   /* largest n for which a_cache computed  */

static uint32_t *spf = NULL;       /* smallest prime factor for each number  */
static size_t    spf_size = 0;     /* allocated size of spf                  */
static bool      *oddOmega = NULL; /* parity of omega(i) (true=odd)          */
static size_t    oddOmega_size = 0;

/* ---------- helper: allocate/resize array ---------- */
static void *xmalloc(size_t sz)
{
    void *p = malloc(sz);
    if (!p) abort();
    return p;
}

static void *xrealloc(void *p, size_t sz)
{
    void *q = realloc(p, sz);
    if (!q) abort();
    return q;
}

/* ---------- sieve up to N, compute spf and oddOmega ---------- */
static void compute_sieve(idx_t N)
{
    if (N < 1) return;
    if (spf_size <= (size_t)N) {
        spf = (uint32_t *)xrealloc(spf, (N + 1) * sizeof(uint32_t));
        oddOmega = (bool *)xrealloc(oddOmega, (N + 1) * sizeof(bool));
        spf_size = N + 1;
    }
    for (idx_t i = 0; i <= N; ++i) spf[i] = 0;
    spf[1] = 1;
    for (idx_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = (uint32_t)i;
            if (i <= N / i) {             /* avoid overflow */
                for (idx_t j = i * i; j <= N; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    oddOmega[1] = false;                  /* omega(1)=0 even */
    for (idx_t i = 2; i <= N; ++i)
        oddOmega[i] = !oddOmega[i / spf[i]]; /* parity flips when removing one prime */
}

/* ---------- ensure a_cache capacity up to N ---------- */
static void ensure_a_cache(idx_t N)
{
    if (a_cache_size >= (size_t)(N + 1)) return;
    a_cache = (seq_t *)xrealloc(a_cache, (N + 1) * sizeof(seq_t));
    /* zero new part */
    for (size_t i = a_cache_size; i <= (size_t)N; ++i) a_cache[i] = 0;
    a_cache_size = (size_t)(N + 1);
    a_cache[1] = 1;                       /* base case */
}

/* ---------- recursive divisor generator ---------- */
static void rec_div(int idx, idx_t acc, idx_t n_local,
                    int pcnt, idx_t *primes, int *exps, seq_t *sum)
{
    if (idx == pcnt) {
        if (acc > 1 && oddOmega[acc]) {
            seq_t sub = a_cache[n_local / acc];
            *sum += sub;
        }
        return;
    }
    idx_t p = primes[idx];
    idx_t cur = acc;
    for (int e = 0; e <= exps[idx]; ++e) {
        rec_div(idx + 1, cur, n_local, pcnt, primes, exps, sum);
        cur *= p;
    }
}

/* ---------- compute a(n) for a single n ---------- */
static seq_t a_calc(idx_t n)
{
    if (n == 1) return 1;
    idx_t tmp = n;
    idx_t primes[12];
    int expv[12];
    int pcnt = 0;
    while (tmp > 1) {
        uint32_t p = spf[tmp];
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            e++;
        }
        primes[pcnt] = p;
        expv[pcnt] = e;
        pcnt++;
    }
    seq_t sum = 0;
    rec_div(0, 1, n, pcnt, primes, expv, &sum);
    return sum;
}

/* ---------- compute a(1..N) sequentially ---------- */
static void compute_a_up_to(idx_t N)
{
    if (N <= a_cache_max) return;
    if (N >= spf_size) compute_sieve(N);
    ensure_a_cache(N);
    for (idx_t n = a_cache_max + 1; n <= N; ++n) {
        a_cache[n] = a_calc(n);
    }
    a_cache_max = N;
}

/* ---------- the required sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined but harmless return */
    compute_a_up_to(n);
    return a_cache[n];
}
