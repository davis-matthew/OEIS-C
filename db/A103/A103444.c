/*
 * A103444  Triangle read by rows: T(n,k) is number of unitary divisors of
 *          C(n,k), 0<=k<=n.
 * https://oeis.org/A103444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^{ω(C(r,k))}, where (r,k) is the row/column of the
 *            triangle read by rows, and ω(m) is the number of distinct prime
 *            divisors of m. The triangle entry T(r,k) is the number of
 *            unitary divisors of the binomial coefficient C(r,k). Unitary
 *            divisors correspond to choosing, for each prime dividin...
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   get_row_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_row_col.h"

/* Legendre's formula: exponent of prime p in n! */
static inline idx_t legendre(idx_t n, idx_t p)
{
    idx_t res = 0;
    for (idx_t div = p; div <= n; div *= p) {
        res += n / div;
        /* Avoid overflow of div in the next loop iteration */
        if (div > n / p) break;
    }
    return res;
}

/* Precompute all primes up to a bound larger than any row number needed
   for n up to the maximal value used by the tests (default 10000).  */
#define MAX_R 200  /* 141 is sufficient for n=10000; 200 is safe */
static int primes[MAX_R / 10 + 10];
static int prime_cnt = 0;
static void init_primes(void)
{
    if (prime_cnt > 0) return;          /* already initialized */
    int sieve[MAX_R + 1];
    for (int i = 2; i <= MAX_R; ++i) sieve[i] = 1;
    for (int i = 2; i * i <= MAX_R; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= MAX_R; j += i) sieve[j] = 0;
        }
    }
    for (int i = 2; i <= MAX_R; ++i) {
        if (sieve[i]) primes[prime_cnt++] = i;
    }
}

/* Main function computing the triangle entry at index n */
seq_t a(idx_t n)
{
    idx_t r, k;
    get_row_col(n, &r, &k);

    init_primes();

    /* Count distinct primes dividing C(r,k) */
    int distinct = 0;
    for (int i = 0; i < prime_cnt && (idx_t)primes[i] <= r; ++i) {
        idx_t p = (idx_t)primes[i];
        idx_t e = legendre(r, p) - legendre(k, p) - legendre(r - k, p);
        if (e > 0) ++distinct;
    }

    /* 2^distinct fits into seq_t for all expected input ranges */
    return ((seq_t)1) << distinct;
}
