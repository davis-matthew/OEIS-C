/*
 * A094236  Triangle read by rows: T(n,k) is the number of standard tableaux
 *          of shape (n,n,k) (0<=k<=n).
 * https://oeis.org/A094236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   add_factor, pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A094236.  The values are read row‑by‑row from the triangle
   T(n,k) = (2n+k)! (n-k+2)(n-k+1) / [k! (n+2)! (n+1)!]  (0 ≤ k ≤ n).
   For a given index N we first recover the row r and column k:
       r = floor((sqrt(8N+1)-1)/2)
       k = N - r(r+1)/2
   Then the function returns T(r,k).
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* bring in the computational helpers used below */
#include "../utils/pow_seq.h"
#include "../utils/seq_mul.h"
#include "../utils/add_factor.h"

/* integer square‑root: floor(sqrt(x)) */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0;
    if (x >= 4611686018427387904ULL) { /* sqrt of max 64‑bit */
        r = 3037000499ULL;
    } else {
        uint64_t hi = 1ULL << 32;
        uint64_t lo = 0;
        while (lo < hi) {
            uint64_t mid = (lo + hi + 1) >> 1;
            if (mid > 0 && mid <= x / mid)
                lo = mid;
            else
                hi = mid - 1;
        }
        r = lo;
    }
    return r;
}

/* Legendre exponent of prime p in m! */
static int legendre_factorial(uint64_t m, uint64_t p)
{
    int e = 0;
    while (m) {
        m /= p;
        e += (int)m;
    }
    return e;
}

/* Main sequence function */
seq_t a(idx_t N)
{
    /* recover row r and column k from index N */
    uint64_t val = 8ULL * N + 1ULL;
    uint64_t s   = isqrt_u64(val);
    uint64_t r   = (s - 1) / 2;

    uint64_t tri = r * (r + 1) / 2;
    if (tri > N) {
        /* adjust downwards in case of rounding */
        while (tri > N) {
            --r;
            tri = r * (r + 1) / 2;
        }
    } else {
        while ((r + 1) * (r + 2) / 2 <= N) {
            ++r;
        }
    }
    uint64_t k = N - r * (r + 1) / 2;

    /* parameters of the factorials involved */
    uint64_t two_n    = 2ULL * r;
    uint64_t two_n_k  = two_n + k;
    uint64_t n_plus_2 = r + 2;
    uint64_t n_plus_1 = r + 1;
    uint64_t nk_plus_2 = r - k + 2;
    uint64_t nk_plus_1 = r - k + 1;

    /* maximum size needed for primes list */
    uint64_t maxP = two_n_k;
    if (maxP < n_plus_2) maxP = n_plus_2;
    if (maxP < n_plus_1) maxP = n_plus_1;
    if (maxP < nk_plus_2) maxP = nk_plus_2;
    if (maxP < nk_plus_1) maxP = nk_plus_1;
    if (maxP < 2)
        maxP = 2;

    /* sieve of Eratosthenes up to maxP */
    uint8_t *is_prime = (uint8_t*)malloc((size_t)(maxP + 1));
    if (!is_prime) return (seq_t)0;           /* allocation failure */
    for (uint64_t i = 0; i <= maxP; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (uint64_t i = 2; i * i <= maxP; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= maxP; j += i)
                is_prime[j] = 0;

    /* collect primes */
    uint64_t *prime = (uint64_t*)malloc((size_t)(maxP + 1) * sizeof(uint64_t));
    int primes_cnt = 0;
    for (uint64_t i = 2; i <= maxP; ++i)
        if (is_prime[i])
            prime[primes_cnt++] = i;

    /* exponents for each prime in the factorisation of the result */
    int *exp = (int*)calloc((size_t)primes_cnt, sizeof(int));

    /* assemble exponent vector:
       e = expFact(2n+k) - expFact(k) - expFact(n+2) - expFact(n+1)
           + factors of (n-k+2) + factors of (n-k+1)
     */
    for (int i = 0; i < primes_cnt; ++i) {
        uint64_t p = prime[i];
        int e = legendre_factorial(two_n_k, p)
            - legendre_factorial(k, p)
            - legendre_factorial(n_plus_2, p)
            - legendre_factorial(n_plus_1, p);
        exp[i] = e;
    }

    add_factor(nk_plus_2, prime, exp, primes_cnt);
    add_factor(nk_plus_1, prime, exp, primes_cnt);

    /* compute the final result from the prime exponents */
    seq_t result = (seq_t)1;
    for (int i = 0; i < primes_cnt; ++i) {
        int e = exp[i];
        if (e > 0) {
            seq_t p_pow = pow_seq((seq_t)prime[i], e);
            result = seq_mul(result, p_pow);
        }
    }

    /* free temporary storage */
    free(is_prime);
    free(prime);
    free(exp);

    return result;
}
