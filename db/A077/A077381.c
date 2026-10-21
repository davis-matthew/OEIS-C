/*
 * A077381  Number of squarefree numbers between successive squares
 *          (exclusive).
 * https://oeis.org/A077381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 210, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute A077381: number of square‑free integers between n² and (n+1)².
    a(n) = count_sqfree((n+1)² − 1) – count_sqfree(n²)
    where count_sqfree(x) = Σ_{k ≤ √x} μ(k)·⌊x/k²⌋ (μ = Möbius function)  */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* ------------- helpers ----------------------------------------------------- */

static int8_t *mu = NULL;          /* Möbius values μ(k) for k ≥ 1      */
static size_t mu_size = 0;         /* current size of 'mu' (≥ needed + 1) */
static uint8_t *is_comp = NULL;    /* composite flag for linear sieve    */
static idx_t *primes = NULL;       /* list of primes found               */
static size_t primes_cap = 0;

/* integer square root: floor(sqrt(x))  */
static inline idx_t isqrt_idx(idx_t x)
{
    if (x < 2) return x;
    idx_t r = (idx_t) sqrt((long double) x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* Ensure μ values up to 'limit' (inclusive) are available. */
static void ensure_mu(idx_t limit)
{
    if (limit < 1) limit = 1;
    if (mu_size >= limit + 1) return;     /* already available */

    /* Resize arrays */
    mu = (int8_t *)realloc(mu, (limit + 1) * sizeof(int8_t));
    is_comp = (uint8_t *)realloc(is_comp, (limit + 1) * sizeof(uint8_t));
    for (size_t i = 0; i <= limit; ++i) is_comp[i] = 0;

    if (primes_cap < limit + 1) {
        primes_cap = limit + 1;
        primes = (idx_t *)realloc(primes, primes_cap * sizeof(idx_t));
    }

    /* Linear sieve for μ */
    size_t primes_cnt = 0;
    mu[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            idx_t p = primes[j];
            idx_t v = i * p;
            if (v > limit) break;
            is_comp[v] = 1;
            if (i % p == 0) {
                mu[v] = 0;
                break;
            } else {
                mu[v] = -mu[i];
            }
        }
    }
    mu_size = limit + 1;
}

/* Count square‑free integers ≤ x. Works for x up to 2⁶⁴‑1.  */
static seq_t count_sqfree(idx_t x)
{
    if (x == 0) return 0;
    idx_t lim = isqrt_idx(x);
    ensure_mu(lim);
    seq_t sum = 0;
    for (idx_t k = 1; k <= lim; ++k) {
        int8_t mk = mu[k];
        if (mk == 0) continue;
        idx_t cnt = x / (k * k);
        if (mk > 0) sum += (seq_t) cnt;
        else        sum -= (seq_t) cnt;
    }
    return sum;
}

/* ------------- sequence function -------------------------------------------- */

/*  a(1) = 2 is the first term (offset 1).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* only defined for n ≥ 1 */

    /* Compute (n+1)² – 1 safely in 128 bits */
    unsigned __int128 up128 = (unsigned __int128)(n + 1) * (n + 1);
    idx_t upper = (idx_t)(up128 - 1);

    /* Compute n² safely */
    unsigned __int128 lo128 = (unsigned __int128)n * n;
    idx_t lower = (idx_t)lo128;

    seq_t su = count_sqfree(upper);
    seq_t sl = count_sqfree(lower);
    return su - sl;
}
