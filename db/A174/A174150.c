/*
 * A174150  Triangle T(n, k) = round(c(n)/(c(k)*c(n-k))) where c(n) = ((n-1)!
 *          * n! * (n+1)!)/ 2^(n-1) if n >= 2, otherwise 1, read by rows.
 * https://oeis.org/A174150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 39, u32 n <= 109, u64 n <= 335,
 *            u128 n <= 1147, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) = round(c(n)/(c(k)*c(n-k))) where
   c(n) = ((n-1)! * n! * (n+1)!)/2^(n-1) for n ≥ 2, c(0)=c(1)=1.
   a(n) is T(r,k) read by rows:  row r = ⌊(√(8n+1)-1)/2⌋,
   column k = n - r(r+1)/2.  */

/* Avoid unnecessary headers; rely on seq_t and idx_t definitions
   provided by the build.  */
#include <stdint.h>
#include <stddef.h>

/* Maximum row we expect (sqrt(2 * 10000) ≈ 142).  Increase for safety. */
#define MAX_ROW    300
#define MAX_PRIME_N 500

static int primes[MAX_PRIME_N];
static size_t primes_cnt = 0;
static int primes_initialized = 0;

/* Simple sieve to fill primes[0..cnt-1] up to 'limit' (inclusive). */
static void init_primes(idx_t limit)
{
    if (limit < 2) return;
    if (limit >= MAX_PRIME_N) limit = MAX_PRIME_N - 1;
    for (idx_t i = 0; i <= limit; ++i) primes[i] = 1;
    primes[0] = primes[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (primes[i])
            for (idx_t j = i * i; j <= limit; j += i)
                primes[j] = 0;
    primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (primes[i]) primes[primes_cnt++] = (int)i;
    primes_initialized = 1;
}

/* Return exponent of prime p in m! (Legendre's formula). */
static int exp_factorial(idx_t m, int p)
{
    int exp = 0;
    while (m)
    {
        m /= p;
        exp += (int)m;
    }
    return exp;
}

/* Compute the exponent vector of c(n) for all primes already
   initialized into the global array.  The output vector must
   have length primes_cnt.  */
static void compute_c_exp(idx_t n, int *exp_out)
{
    for (size_t i = 0; i < primes_cnt; ++i)
        exp_out[i] = 0;

    if (n < 2) return;          /* c(n)=1 => all exponents 0 */

    for (size_t i = 0; i < primes_cnt; ++i)
    {
        int p = primes[i];
        int e = exp_factorial(n - 1, p)
              + exp_factorial(n,     p)
              + exp_factorial(n + 1, p);
        if (p == 2) e -= (int)(n - 1);   /* division by 2^(n-1) */
        exp_out[i] = e;
    }
}

/* Compute T(n,k) using prime exponent subtraction. */
static seq_t compute_T(idx_t n, idx_t k)
{
    if (k == 0 || k == n) return 1;

    /* Ensure primes up to max(n, k, n-k) are available. */
    idx_t max_n = n;
    if (k > max_n) max_n = k;
    if (n - k > max_n) max_n = n - k;
    if (!primes_initialized || max_n >= primes[primes_cnt - 1])
        init_primes(max_n + 1);

    int exp_n[primes_cnt];
    int exp_k[primes_cnt];
    int exp_nk[primes_cnt];
    compute_c_exp(n,   exp_n);
    compute_c_exp(k,   exp_k);
    compute_c_exp(n - k, exp_nk);

    seq_t result = 1;
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        int e = exp_n[i] - exp_k[i] - exp_nk[i];
        for (int j = 0; j < e; ++j)
            result *= (seq_t)primes[i];
    }
    return result;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* Find row r and column k corresponding to linear index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Compute the triangle entry. */
    return compute_T(r, k);
}
