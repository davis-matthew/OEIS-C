/*
 * A238892  Index of last squarefree number in the first half of row n of
 *          Pascal's triangle.
 * https://oeis.org/A238892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 564, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A238892: index of the last squarefree binomial coefficient in
   the first half of row n of Pascal's triangle.  No closed formula is
   known; the function searches from the middle downwards using a
   pre‑computed table of prime exponents in factorials. */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>     /* for sqrt(double) */

static idx_t cached_n = 0ULL;      /* largest n for which tables are built */
static idx_t prime_count = 0ULL;   /* number of primes ≤ cached_n          */
static uint32_t *primes = NULL;    /* array of primes                      */
static uint16_t *vfac   = NULL;    /* vfac[pi][i] = exponent of primes[pi] in i! */

/* ------------------------------------------------------------------
   Build tables for all n ≤ new_n: list of primes and table of factorial
   prime exponents via incremental accumulation.
   ------------------------------------------------------------------ */
static void build_tables(idx_t new_n)
{
    /* Sieve of Eratosthenes up to new_n */
    idx_t sz = new_n + 1;
    char *is_comp = (char *)malloc(sz * sizeof(char));
    if (!is_comp) return;                   /* memory failure – abort */

    for (idx_t i = 0; i < sz; ++i) is_comp[i] = 0;

    idx_t max_p = (idx_t)sqrt((double)new_n);
    idx_t count = 0;
    for (idx_t i = 2; i <= new_n; ++i) {
        if (!is_comp[i]) {
            if (i <= max_p) {
                for (idx_t j = i * i; j <= new_n; j += i) is_comp[j] = 1;
            }
            ++count;
        }
    }

    /* Reallocate primes array */
    uint32_t *new_primes = (uint32_t *)realloc(primes, count * sizeof(uint32_t));
    if (!new_primes) {
        free(is_comp);
        return;
    }
    primes = new_primes;
    prime_count = count;
    idx_t pidx = 0;
    for (idx_t i = 2; i <= new_n; ++i)
        if (!is_comp[i]) primes[pidx++] = (uint32_t)i;

    free(is_comp);

    /* Allocate and compute factorial prime exponents table */
    idx_t rows = prime_count;
    idx_t cols = new_n + 1;
    uint16_t *new_vfac = (uint16_t *)realloc(vfac, rows * cols * sizeof(uint16_t));
    if (!new_vfac) return;
    vfac = new_vfac;

    for (idx_t pi = 0; pi < rows; ++pi) {
        uint32_t p = primes[pi];
        idx_t base = pi * cols;
        new_vfac[base + 0] = 0;
        for (idx_t i = 1; i <= new_n; ++i) {
            uint32_t t = (uint32_t)i;
            uint16_t c = 0;
            while (t % p == 0) { c++; t /= p; }
            new_vfac[base + i] = new_vfac[base + i - 1] + c;
        }
    }
    cached_n = new_n;
}

/* ------------------------------------------------------------------
   Test whether binomial coefficient C(n,k) is squarefree using the
   precomputed factorial prime exponent table.
   ------------------------------------------------------------------ */
static int is_squarefree(idx_t n, idx_t k)
{
    idx_t n_k = n - k;
    idx_t cols = cached_n + 1;
    for (idx_t pi = 0; pi < prime_count; ++pi) {
        idx_t base = pi * cols;
        uint16_t exp = vfac[base + n] - vfac[base + k] - vfac[base + n_k];
        if (exp > 1) return 0;          /* contains a square prime factor */
    }
    return 1;  /* squarefree */
}

/* ------------------------------------------------------------------
   Main function as required.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n > cached_n) build_tables(n);

    if (n == 0) return (seq_t)0;   /* C(0,0)=1 is squarefree */

    idx_t half = n >> 1;          /* floor(n/2) */
    for (idx_t k = half; ; --k) {
        if (is_squarefree(n, k)) return (seq_t)k;
        if (k == 0) break;
    }
    return (seq_t)0;   /* should never reach here */
}
