/*
 * A131440  Triangular table of numerators of the coefficients of
 *          Laguerre-Sonin polynomials L(1/2,n,x).
 * https://oeis.org/A131440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, frac, tabl
 * Formula:   a(n) = numerator of (-1)^m * binomial(r+1/2, r-m) / m!, where r
 *            = floor((sqrt(1+8n)-1)/2) and m = n - r(r+1)/2
 * Range:     i8 n <= 9, i16 n <= 35, i32 n <= 135, i64 n <= 466,
 *            i128 n <= 1833, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for values up to 2^67 using 128‑bit arithmetic. */
static idx_t isqrt_u128(unsigned __int128 x) {
    unsigned long long low = 0, high = ((unsigned long long)1 << 32);
    while (low < high) {
        unsigned long long mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Small prime sieve up to 400. */
static unsigned int primes[400];
static int prime_cnt;
static signed char prime_index[401];
static int init = 0;

static void init_primes(void) {
    if (init) return;
    int limit = 400;
    signed char is_prime[401];
    for (int i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (int i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= limit; j += i) is_prime[j] = 0;
    prime_cnt = 0;
    for (int i = 2; i <= limit; ++i)
        if (is_prime[i]) {
            primes[prime_cnt++] = (unsigned int)i;
            prime_index[i] = prime_cnt - 1;
        }
    for (int i = limit + 1; i <= 400; ++i)
        prime_index[i] = -1;
    init = 1;
}

/* Factorise val and add sign*exponent to exp[prime]. */
static void factorize(unsigned long long val, int sign,
                      int *exp)
{
    for (int i = 0; i < prime_cnt; ++i) {
        unsigned int p = primes[i];
        if ((unsigned long long)p * p > val) break;
        int cnt = 0;
        while (val % p == 0) {
            val /= p;
            ++cnt;
        }
        if (cnt) exp[i] += cnt * sign;
    }
    if (val > 1) {
        /* val is prime and <= 400 */
        int idx = prime_index[(unsigned int)val];
        if (idx >= 0) exp[idx] += sign;
    }
}

seq_t a(idx_t n) {
    init_primes();

    /* Determine row r and column m in the triangle. */
    idx_t r = (isqrt_u128((unsigned __int128)1 + (unsigned __int128)8 * n) - 1) / 2;
    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t m = n - offset;       /* 0 <= m <= r */
    idx_t k = r - m;            /* k = r - m */

    /* Exponent array for primes in the simplified fraction. */
    int exp[prime_cnt];
    for (int i = 0; i < prime_cnt; ++i) exp[i] = 0;

    /* numerator factors: product of odd numbers (2r+1-2i) for i=0..k-1 */
    for (idx_t i = 0; i < k; ++i) {
        unsigned long long val = 2ULL * r + 1 - 2ULL * i;
        factorize(val, +1, exp);
    }

    /* denominator factors. */
    /* 2^k */
    int idx2 = prime_index[2];
    if (idx2 >= 0) exp[idx2] -= (int)k;

    /* k! */
    for (idx_t i = 2; i <= k; ++i) factorize(i, -1, exp);

    /* m! */
    for (idx_t i = 2; i <= m; ++i) factorize(i, -1, exp);

    /* Assemble the numerator. */
    seq_t result = 1;
    for (int i = 0; i < prime_cnt; ++i) {
        int cnt = exp[i];
        if (cnt > 0) {
            unsigned int p = primes[i];
            for (int c = 0; c < cnt; ++c) result *= (seq_t)p;
        }
    }

    /* Sign = (-1)^m */
    if ((m & 1) != 0) result = -result;

    return result;
}
