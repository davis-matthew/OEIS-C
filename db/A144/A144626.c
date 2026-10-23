/*
 * A144626  Tetrahedron of numbers T(i,j,k) = (i+2*j+3*k)!/(i!*j!*k!*2^j*6^k)
 *          read with entries in the order defined in A144625.
 * https://oeis.org/A144626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(i,j,k) where (i,j,k) are obtained from n by the
 *            enumeration described in A144625: For each n, find the unique
 *            triple (i,j,k) with i+j+k = s, 0 ≤ k ≤ s, 0 ≤ i ≤ s-k, j =
 *            s-i-k, such that the linear index of (i,j,k) equals n. T(i,j,k)
 *            = (i+2j+3k)! / (i! j! k! 2^j 6^k)
 * Range:     u8 n <= 17, u16 n <= 47, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow_factor.h"

/* Primes up to 30000 are sufficient for all n ≤ 10000 (max S = 3n) */
#define PRIME_LIMIT   30000
#define MAX_PRIME_CNT 4000   /* > number of primes <= PRIME_LIMIT */

/* Static storage for primes and flag to initialise once */
static uint64_t primes[MAX_PRIME_CNT];
static size_t   prime_count = 0;
static bool     primes_inited = false;

/* Initialise the primes array once */
static void init_primes(void)
{
    if (primes_inited) return;
    primes_inited = true;

    bool is_prime[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t p = 2; p * p <= PRIME_LIMIT; ++p)
        if (is_prime[p])
            for (size_t q = p * p; q <= PRIME_LIMIT; q += p)
                is_prime[q] = false;

    for (size_t p = 2; p <= PRIME_LIMIT; ++p)
        if (is_prime[p]) {
            if (prime_count < MAX_PRIME_CNT)
                primes[prime_count++] = p;
        }
}

/* Legendre\'s formula: exponent of prime p in N! */
static int exp_in_fact(idx_t N, uint64_t p)
{
    int e = 0;
    idx_t div = p;
    while (div <= N) {
        e += N / div;
        if (div > N / p) break;   /* avoid overflow */
        div *= p;
    }
    return e;
}

/* T(i,j,k) as defined in the OEIS entry */
static seq_t compute_T(idx_t i, idx_t j, idx_t k)
{
    init_primes();

    /* S = i + 2j + 3k */
    idx_t S = i + 2 * j + 3 * k;
    seq_t result = 1;

    for (size_t pi = 0; pi < prime_count; ++pi) {
        uint64_t p = primes[pi];
        int e = exp_in_fact(S, p)
              - exp_in_fact(i, p)
              - exp_in_fact(j, p)
              - exp_in_fact(k, p);

        /* subtract extra factors from the denominators 2^j 6^k */
        if (p == 2) e -= (int)(j + k);
        else if (p == 3) e -= (int)k;

        if (e <= 0) continue;
        result *= pow_factor((seq_t)p, e);
    }

    return result;
}

/* Helper: C(n,3) for n ≥ 3, returns 0 if n < 3 */
static idx_t binom3(idx_t n)
{
    if (n < 3) return 0;
    return (n * (n - 1) * (n - 2)) / 6;
}

/* Accepts n and returns the corresponding sequence value */
seq_t a(idx_t n)
{
    /* Find slice s such that C(s+3,3) > n */
    idx_t s = 0;
    while (binom3(s + 3) <= n) ++s;

    /* Offset of the first term in this slice */
    idx_t offset = binom3(s + 2);
    idx_t t = n - offset;            /* position within the slice */

    /* Determine k by subtracting row lengths */
    idx_t k = 0;
    while (t >= (s - k + 1)) {
        t -= (s - k + 1);
        ++k;
    }

    /* i and j from the remaining position */
    idx_t i = (s - k) - t;
    idx_t j = s - i - k;

    return compute_T(i, j, k);
}
