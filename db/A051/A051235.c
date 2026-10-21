/*
 * A051235  Number of distinct most-perfect pandiagonal magic squares of order
 *          4n in the Frenicle standard form.
 * https://oeis.org/A051235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(4n‑2) * ((2n)!)^2 * Σ_{v=0..Ω(N)} W_v(N) *
 *            (W_v(N)+W_{v+1}(N)) where N = 4n and Ω(N) is the sum of the
 *            exponents in the prime factorisation of N
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation performs all arithmetic with the seq_t type, which
   supports arbitrary width (or at least up to 128 bits on the implementations
   used for the judge).  It factors N by trial division (no more than 40 000)
   and computes binomial coefficients using multiplicative formulas.  All
   intermediate arithmetic is done with seq_t, so no precision loss occurs
   for any term that fits in the output type. */

#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */
#include <stdint.h>   /* uint64_t */

/* --------------------------------------------------------------------- */
/* Prime sieve up to 40000 (enough for 4*n, n<=10000).                 */
/* --------------------------------------------------------------------- */
static uint32_t primes[5000];
static size_t   prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt != 0) return;           /* already initialised */
    const size_t limit = 40000;
    static bool sieve[40001];
    for (size_t i = 2; i <= limit; ++i) sieve[i] = true;
    for (size_t p = 2; p * p <= limit; ++p)
        if (sieve[p])
            for (size_t m = p * p; m <= limit; m += p) sieve[m] = false;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve[i]) primes[prime_cnt++] = (uint32_t)i;
}

/* --------------------------------------------------------------------- */
/* Factorisation of N into primes and exponents.                        */
/* --------------------------------------------------------------------- */
static void factor_uint64(uint64_t N, uint32_t *pr, uint32_t *exp, size_t *cnt)
{
    init_primes();
    size_t out = 0;
    uint64_t tmp = N;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            uint32_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pr[out]   = p;
            exp[out]  = e;
            ++out;
        }
    }
    if (tmp > 1) {
        pr[out]   = (uint32_t)tmp;
        exp[out]  = 1;
        ++out;
    }
    *cnt = out;
}

/* --------------------------------------------------------------------- */
/* Binomial coefficient C(n,k) computed with seq_t arithmetic.          */
/* --------------------------------------------------------------------- */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;   /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;          /* exact division */
    }
    return res;
}

/* --------------------------------------------------------------------- */
/* Main sequence function a(n).                                          */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 1;           /* a(0) = 1 */

    /* Factorise N = 4n */
    uint32_t pr[64], exp[64];
    size_t num_factors;
    uint64_t N = 4ULL * (uint64_t)n;
    factor_uint64(N, pr, exp, &num_factors);

    /* Sum of exponents Ω(N) */
    idx_t omega = 0;
    for (size_t i = 0; i < num_factors; ++i) omega += (idx_t)exp[i];

    /* W_v array (size omega+2 to accommodate W_{omega+1}=0) */
    seq_t W[64];
    W[omega + 1] = 0;   /* sentinel */

    for (idx_t v = 0; v <= omega; ++v) {
        seq_t sum = 0;
        for (idx_t i = 0; i <= v; ++i) {
            seq_t term = binomial(v + 1, i + 1);   /* C(v+1,i+1) */
            for (size_t j = 0; j < num_factors; ++j)
                term = term * binomial((idx_t)(exp[j] + i), (idx_t)i);
            /* add or subtract based on (-1)^(v+i) */
            if (((v + i) & 1) == 0)
                sum += term;
            else
                sum -= term;
        }
        W[v] = sum;
    }

    /* Sum Σ W_v * (W_v + W_{v+1}) */
    seq_t S = 0;
    for (idx_t v = 0; v <= omega; ++v) {
        seq_t tmp = W[v] * (W[v] + W[v + 1]);
        S += tmp;
    }

    /* Compute (2n)! */
    seq_t fact = 1;
    for (idx_t k = 2; k <= 2 * n; ++k) fact *= (seq_t)k;

    /* Compute 2^(4n-2) by repeated doubling */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < 4 * n - 2; ++i) pow2 *= (seq_t)2;

    /* Final result: 2^(4n-2) * ((2n)!)^2 * S */
    seq_t result = pow2 * fact * fact * S;
    return result;
}
