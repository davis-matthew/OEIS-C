/*
 * A212072  G.f. satisfies: A(x) = (1 + x*A(x)^2)^3.
 * https://oeis.org/A212072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*binomial(6*n+3,n)/(6*n+3) = binomial(6*n+3,n)/(2*n+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the formula using prime‐factorisation.  The computation is
   kept inside the range of the result for all supported build settings. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/pow_seq.h"

#define MAX_N 60003  /* 6*10000+3, enough for n <= 10000 */

static int spf[MAX_N + 1];          /* smallest prime factor for each number */
static unsigned char prime_exp[MAX_N + 1];
static int primes_arr[MAX_N / 10 + 1]; /* enough space for all primes up to MAX_N */
static size_t primes_cnt = 0;
static int init_done = 0;

/* Utility: compute smallest prime factor sieve once */
static void init_spf(void)
{
    if (init_done) return;
    for (int i = 0; i <= MAX_N; ++i) spf[i] = 0;
    for (int i = 2; i <= MAX_N; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes_arr[primes_cnt++] = i;
            if ((int64_t)i * i <= MAX_N)
                for (int j = i * i; j <= MAX_N; j += i)
                    if (spf[j] == 0) spf[j] = i;
        }
    }
    init_done = 1;
}

/* Legendre's formula: exponent of prime p in n! */
static inline idx_t legendre(idx_t n, int p)
{
    idx_t t = n;
    uint64_t sum = 0;
    while (t) {
        t /= p;
        sum += t;
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    init_spf();                    /* ensure sieve is ready */

    idx_t N = 6 * n + 3;            /* upper bound of factorial arguments */
    idx_t M = 5 * n + 3;            /* M = 5n+3, product range is (5n+4 .. 6n+3) */

    /* Clear exponent array for current n */
    for (size_t i = 0; i < primes_cnt; ++i) prime_exp[primes_arr[i]] = 0;

    /* Compute exponents from the product (5n+4 .. 6n+3) */
    for (size_t i = 0; i < primes_cnt; ++i) {
        int p = primes_arr[i];
        if (p > N) break;
        uint64_t exp = legendre(N, p) - legendre(M, p) - legendre(n, p);
        prime_exp[p] = (unsigned char)exp;
    }

    /* Subtract exponents of the single denominator factor (2n+1) */
    idx_t tmp = 2 * n + 1;
    while (tmp > 1) {
        int p = spf[tmp];
        int cnt = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
        }
        uint8_t cur = prime_exp[p];
        if (cur < cnt)          /* should not happen for valid n */
            prime_exp[p] = 0;
        else
            prime_exp[p] = cur - cnt;
    }

    /* Build the result from the remaining prime exponents */
    seq_t result = 1;
    for (size_t i = 0; i < primes_cnt; ++i) {
        int p = primes_arr[i];
        if (p > N) break;
        uint8_t e = prime_exp[p];
        if (e > 0) {
            result *= pow_seq((seq_t)p, e);
        }
    }
    return result;
}
