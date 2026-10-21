/*
 * A078534  Coefficients of power series that satisfies A(x)^5 - 25*x*A(x)^6 =
 *          1, A(0)=1.
 * https://oeis.org/A078534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^n * prod_{k=0}^{n-1} (6n-4-5k) / ((n+1)*n!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_N_LIMIT 10000          /* maximum n expected (≈10 000)      */
#define MAX_PRIME   (6 * MAX_N_LIMIT + 10)   /* 6*n + 10  →  <= 60010 */

static int primes[MAX_PRIME + 1];
static size_t primes_cnt = 0;
static int primes_ready = 0;

/* Simple sieve - computed only once */
static void init_primes(void)
{
    if (primes_ready) return;
    size_t limit = MAX_PRIME;
    unsigned char *is_p = (unsigned char*)calloc(limit + 1, 1);
    if (!is_p) exit(1);
    memset(is_p, 1, limit + 1);
    is_p[0] = is_p[1] = 0;
    for (size_t i = 2; i * i <= limit; ++i)
        if (is_p[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_p[j] = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_p[i]) primes[primes_cnt++] = (int)i;
    free(is_p);
    primes_ready = 1;
}

/* Binary search in the primes array */
static size_t prime_index(int p)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] >= p) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Ensure primes up to 6*n are ready */
    init_primes();

    /* Allocate array of exponents for each prime */
    int *exp = (int*)calloc(primes_cnt, sizeof(int));
    if (!exp) exit(1);

    /* 1. Add exponents coming from the product terms */
    for (idx_t k = 0; k < n; ++k) {
        int term = 6 * (int)n - 4 - 5 * (int)k; /* term > 0 for k < n */
        int x = term;
        for (size_t pi = 0; pi < primes_cnt && primes[pi] * primes[pi] <= x; ++pi) {
            int p = primes[pi];
            while (x % p == 0) {
                ++exp[pi];
                x /= p;
            }
        }
        if (x > 1) {                     /* x itself is prime */
            size_t idx = prime_index(x);
            ++exp[idx];
        }
    }

    /* 2. Add exponent of 5^n */
    size_t idx5 = prime_index(5);
    exp[idx5] += (int)n;

    /* 3. Subtract exponents from n! */
    int nn = (int)n;
    for (size_t pi = 0; pi < primes_cnt && primes[pi] <= nn; ++pi) {
        int p = primes[pi];
        int t = nn / p;
        while (t) {
            exp[pi] -= t;
            t /= p;
        }
    }

    /* 4. Subtract exponents coming from (n+1) */
    int x = nn + 1;
    for (size_t pi = 0; pi < primes_cnt && primes[pi] * primes[pi] <= x; ++pi) {
        int p = primes[pi];
        while (x % p == 0) {
            --exp[pi];
            x /= p;
        }
    }
    if (x > 1) {                         /* remaining prime factor */
        size_t idx = prime_index(x);
        --exp[idx];
    }

    /* 5. Construct the result from prime powers */
    seq_t result = (seq_t)1;
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        int e = exp[pi];
        if (e > 0) {
            seq_t p = (seq_t)primes[pi];
            for (int k = 0; k < e; ++k) result *= p;
        }
    }

    free(exp);
    return result;
}
