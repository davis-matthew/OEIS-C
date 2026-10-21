/*
 * A073044  Triangle read by rows: T(n,k) (n >= 1, n-1 >= k >= 0) = number of
 *          n-sequences of 0's and 1's with no pair of adjacent 0's and
 *          exactly k pairs of adjacent 1's.
 * https://oeis.org/A073044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ceil((sqrt(8n+1)-1)/2), k = n - r(r-1)/2
 *            - 1. T(r,k) = binom( floor((r+k-1)/2), k ) + binom(
 *            floor((r+k-2)/2), k )
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* --- Prime list cache ---------------------------------------------- */
static idx_t *prime_list = NULL;   /* dynamic array of primes */
static size_t prime_count = 0;     /* number of primes stored */

/* Ensure that prime_list contains all primes ≤ limit.  */
static void
ensure_primes(idx_t limit)
{
    if (prime_list && prime_count > 0 && prime_list[prime_count - 1] >= limit)
        return;                      /* already up to date */

    /* Grow storage if needed.  */
    if (!prime_list) {
        size_t cap = (size_t)limit + 2;
        prime_list = (idx_t *)malloc(cap * sizeof(idx_t));
        prime_count = 0;
    }

    /* Add missing primes.  */
    idx_t start = (prime_count > 0 ? prime_list[prime_count - 1] + 1 : 2);
    for (idx_t n = start; n <= limit; ++n) {
        int is_p = 1;
        for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= n; ++i)
            if (n % prime_list[i] == 0) { is_p = 0; break; }
        if (is_p) {
            prime_list[prime_count++] = n;
        }
    }
}

/* Exponent of prime p in m!  (Legendre formula) */
static int
exp_factorial(idx_t m, idx_t p)
{
    int exp = 0;
    while (m) {
        m /= p;
        exp += (int)m;
    }
    return exp;
}

/* Binomial coefficient via prime factorisation.  */
static seq_t
binomial_by_primes(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;         /* symmetry */

    ensure_primes(n);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < prime_count && prime_list[i] <= n; ++i) {
        idx_t p = prime_list[i];
        int e = exp_factorial(n, p) - exp_factorial(k, p) - exp_factorial(n - k, p);
        for (int j = 0; j < e; ++j)
            res *= (seq_t)p;
    }
    return res;
}

/* Main sequence function --------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* out of offset, value irrelevant */

    /* Find the row r such that 1+2+...+r >= n (cumulative count). */
    idx_t r = 1;
    idx_t cumulative = 1;
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Column k within row r (0‑based). */
    idx_t start_index = (r - 1) * r / 2;   /* first index of this row */
    idx_t k = n - start_index - 1;

    /* Compute T(r,k) by the binomial formula. */
    idx_t a1 = (r + k - 1) / 2;        /* floor((r+k-1)/2) */
    idx_t a2 = (r + k - 2) / 2;        /* floor((r+k-2)/2) */

    seq_t t1 = binomial_by_primes(a1, k);
    seq_t t2 = binomial_by_primes(a2, k);
    return t1 + t2;
}
