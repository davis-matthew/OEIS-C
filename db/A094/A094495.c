/*
 * A094495  Table of binomial coefficients mod m^2, read by rows: T(m, n) =
 *          binomial(m, n) mod m^2.
 * https://oeis.org/A094495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = binomial(m, k) mod m^2,
 * where m,k are the unique row and column indices for the n-th term
 * read row‑by‑row from m = 0,1,2,… and k = 0…m.
 * The index n = 0 corresponds to the convention a(0) = 0.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helpers from utils */
#include "../utils/mod_pow.h"
#include "../utils/build_primes_upto.h"

static bool primes_initialized = false;
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;

/* Exponent of prime p in n! */
static idx_t exp_factorial(idx_t n, idx_t p)
{
    idx_t e = 0;
    while (n >= p) {
        n /= p;
        e += n;
    }
    return e;
}

/* Compute binomial(m,k) mod m^2 without overflow. */
static seq_t binom_mod(idx_t m, idx_t k)
{
    seq_t mod = (seq_t)m * (seq_t)m;
    seq_t res = 1;

    /* Ensure we have primes up to m. */
    if (!primes_initialized || m >= prime_cnt) {
        if (primes) free(primes);
        build_primes_upto(m, &primes, &prime_cnt);
        primes_initialized = true;
    }

    for (size_t idx = 0; idx < prime_cnt && primes[idx] <= m; ++idx) {
        idx_t p = (idx_t)primes[idx];
        idx_t e = exp_factorial(m, p) - exp_factorial(k, p) - exp_factorial(m - k, p);
        if (e == 0) continue;
        seq_t pp = mod_pow((seq_t)p, (idx_t)e, mod);
        res = (res * pp) % mod;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* a(0) is defined by convention */
    if (n == 0)
        return 0;

    /* Find the row m such that cumulative terms up to m >= n+1 */
    idx_t m = 0;
    idx_t cum = 1;                /* cum = (m+1)(m+2)/2 for m=0 */
    while (cum <= n) {
        ++m;
        cum += m + 1;
    }

    /* Number of terms before row m */
    idx_t prev_cum = cum - (m + 1);
    /* Position within the row (0‑based) */
    idx_t k = n - prev_cum;

    /* For endpoints of the row, binomial = 1 */
    if (k == 0 || k == m)
        return 1 % ((seq_t)m * (seq_t)m);

    return binom_mod(m, k);
}
