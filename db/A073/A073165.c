/*
 * A073165  Triangle T(n,k) read by rows: related to David G. Cantor's sigma
 *          function.
 * https://oeis.org/A073165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n‑th term of the triangle T(r,k) read row by row,
 *            where T(r,k)=∏_{1≤i≤j≤k} (n+i+j-1)/(i+j-1) with n=r−k
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 111, u64 n <= 220,
 *            u128 n <= 422, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Prime sieve up to 500 – enough for all arguments needed. */
#define MAX_PRIME 500
static int primes[MAX_PRIME/2];
static int prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    bool is_prime[MAX_PRIME+1];
    for (int i = 0; i <= MAX_PRIME; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= MAX_PRIME; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= MAX_PRIME; j += i)
                is_prime[j] = false;
    for (int i = 2; i <= MAX_PRIME; ++i)
        if (is_prime[i]) primes[prime_cnt++] = i;
    primes_inited = true;
}

/* Raise a to exp (exp ≥ 0) – works for any seq_t. */
static seq_t pow_seq(seq_t a, unsigned int exp)
{
    seq_t res = 1;
    while (exp)
    {
        if (exp & 1) res *= a;
        a *= a;
        exp >>= 1;
    }
    return res;
}

/* Add or subtract prime exponents given by factorization of val */
static void factorise(idx_t val, int sign, int *exps)
{
    idx_t x = val;
    for (int pi = 0; pi < prime_cnt; ++pi)
    {
        int p = primes[pi];
        if ((idx_t)p * (idx_t)p > x) break;
        int cnt = 0;
        while ((x % p) == 0) { x /= p; ++cnt; }
        if (cnt) exps[pi] += sign * cnt;
    }
    if (x > 1)
    {
        /* x is a prime > sqrt(original) – find it in the list. */
        for (int pi = 0; pi < prime_cnt; ++pi)
        {
            if (primes[pi] == (int)x)
            {
                exps[pi] += sign;
                break;
            }
        }
    }
}

/* Compute the n-th term of the sequence. */
seq_t a(idx_t n)
{
    init_primes();

    /* Determine row r and column k (0-based) from the linear index n. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;
    idx_t k = n - r * (r + 1) / 2;
    if (k == 0) return (seq_t)1;

    /* Row r has k columns from 0..r.                           */
    idx_t m = r - k;           /* the n in the product formula.        */
    int exps[prime_cnt];
    for (int i = 0; i < prime_cnt; ++i) exps[i] = 0;

    /* Multiply (m + i + j - 1) / (i + j - 1) for 1 ≤ i ≤ j ≤ k.  */
    for (idx_t i = 1; i <= k; ++i)
    {
        for (idx_t j = i; j <= k; ++j)
        {
            idx_t num = m + i + j - 1;
            idx_t den = i + j - 1;
            factorise(num, +1, exps);
            factorise(den, -1, exps);
        }
    }

    /* Reconstruct the integer from prime exponents. */
    seq_t val = 1;
    for (int pi = 0; pi < prime_cnt; ++pi)
    {
        int e = exps[pi];
        if (e > 0) val *= pow_seq((seq_t)primes[pi], (unsigned int)e);
    }
    return val;
}
