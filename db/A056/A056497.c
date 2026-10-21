/*
 * A056497  Number of primitive (period n) periodic palindromes using a
 *          maximum of six different symbols.
 * https://oeis.org/A056497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * B(n/d) , where B(m) = Σ_{i=0..2}
 *            binom(6,i) * 6^{(m-i)/2} (only when m-i is even)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* maximum N we support for caching and precomputation (sequence is used up to ~10000) */
#define MAX_N 10000

/* global storage for memoized results */
static seq_t cache[MAX_N + 1];
static bool cached[MAX_N + 1] = {0};

/* Möbius values for 1..MAX_N */
static int8_t mu[MAX_N + 1];
static bool mu_initialized = false;

/* dynamic array of powers of 6: pow6[i] = 6^i */
static seq_t *pow6 = NULL;
static size_t pow6_size = 0;   /* number of elements stored */
static size_t pow6_cap  = 0;   /* capacity of the array */

/* ------------------------------------------------------------ */
/* sieve to compute primes and Möbius function up to MAX_N      */
/* ------------------------------------------------------------ */
static void init_mu(void)
{
    if (mu_initialized) return;
    mu_initialized = true;

    /* start with μ[n] = 1 for all n ≥ 1 */
    for (size_t i = 0; i <= MAX_N; ++i) mu[i] = 1;
    mu[0] = 0;  /* μ(0) is undefined, set to 0 */
    mu[1] = 1;

    /* simple sieve to generate primes up to MAX_N */
    bool is_prime[MAX_N + 1];
    for (size_t i = 0; i <= MAX_N; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t p = 2; p <= MAX_N; ++p) {
        if (!is_prime[p]) continue;
        /* multiply μ by -1 for all multiples of p */
        for (size_t j = p; j <= MAX_N; j += p) {
            mu[j] *= -1;
        }
        /* set μ to 0 for all multiples of p² */
        size_t pp = p * p;
        if (pp > MAX_N) continue;
        for (size_t j = pp; j <= MAX_N; j += pp) {
            mu[j] = 0;
        }
        /* mark composites */
        for (size_t j = p * p; j <= MAX_N; j += p) {
            is_prime[j] = false;
        }
    }
}

/* ------------------------------------------------------------ */
/* helper to grow pow6 array up to index 'exp'                   */
/* ------------------------------------------------------------ */
static void ensure_pow6(size_t exp)
{
    if (pow6 == NULL) {
        /* initialise with one element 6^0 = 1 */
        pow6_cap = 1;
        pow6 = (seq_t *)malloc(pow6_cap * sizeof(seq_t));
        pow6[0] = (seq_t)1;
        pow6_size = 1;
    }
    while (pow6_size <= exp) {
        if (pow6_size >= pow6_cap) {
            pow6_cap <<= 1;
            pow6 = (seq_t *)realloc(pow6, pow6_cap * sizeof(seq_t));
        }
        pow6[pow6_size] = pow6[pow6_size - 1] * (seq_t)6;
        pow6_size++;
    }
}

/* ------------------------------------------------------------ */
/* B(m) = number of necklaces of length m with reflect. sym.  */
/* ------------------------------------------------------------ */
static seq_t B_of(size_t m)
{
    if (m & 1) {                 /* m is odd */
        size_t p = (m + 1) / 2;
        ensure_pow6(p);
        return pow6[p];
    } else {                     /* m is even, m >= 2 */
        size_t p = m / 2;
        ensure_pow6(p);
        /* B = 6^p + 15 * 6^(p-1) */
        seq_t term1 = pow6[p];
        seq_t term2 = pow6[p - 1] * (seq_t)15;
        return term1 + term2;
    }
}

/* ------------------------------------------------------------ */
/* main sequence function                                     */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but return 0 */

    if (n <= MAX_N && cached[n]) {
        return cache[n];
    }

    init_mu();                     /* ensure μ is ready */

    seq_t result = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;
        idx_t d1 = i;
        idx_t d2 = n / i;

        /* first divisor d1 */
        int mu_val = mu[d1];
        if (mu_val != 0) {
            idx_t q = n / d1;
            seq_t Bq = B_of(q);
            if (mu_val == 1) result += Bq;
            else result -= Bq;
        }

        /* second divisor d2, avoid double count when i == n/i */
        if (d2 != d1) {
            mu_val = mu[d2];
            if (mu_val != 0) {
                idx_t q = n / d2;
                seq_t Bq = B_of(q);
                if (mu_val == 1) result += Bq;
                else result -= Bq;
            }
        }
    }

    if (n <= MAX_N) {
        cache[n] = result;
        cached[n] = true;
    }
    return result;
}
