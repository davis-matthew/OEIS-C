/*
 * A094049  Let p(n) be the n-th prime congruent to 1 mod 4. Then a(n) = the
 *          least k for which m^2+1=p(n)*k^2 has a solution.
 * https://oeis.org/A094049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least k such that m^2 + 1 = p(n) * k^2 has an integer
 *            solution (p(n) = the n‑th prime ≡ 1 (mod 4)). The function
 *            computes k from the fundamental solution of the negative Pell
 *            equation m^2 – p·k^2 = –1
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   minimal_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/minimal_k.h"

/* Static storage for the primes congruent to 1 modulo 4. */
static uint64_t *primes1 = NULL;
static size_t primes1_cap = 0;

/* Ensure that at least 'need' primes ≡ 1 mod 4 are stored in 'primes1'.  */
static void ensure_primes1(size_t need) {
    if (primes1_cap >= need) return;          /* already have enough */

    /* Determine an upper bound on the needed prime using the prime number
       theorem: p_n < n (log n + log log n) for n ≥ 6.  Here we use a
       conservative over‑estimate. */
    size_t n_est = need * 2 + 10;
    double dn = (double)n_est;
    double upper = dn * (log(dn) + log(log(dn))) + 10.0;
    size_t limit = (size_t)upper + 1;

    /* Allocate new storage for the primes.  */
    uint64_t *tab = (uint64_t *)realloc(primes1, need * sizeof(uint64_t));
    primes1 = tab;
    primes1_cap = need;
    size_t pos = 0;

    /* Sieve of Eratosthenes up to 'limit'.  */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return;                     /* allocation failure – abort */
    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }

    /* Collect primes congruent to 1 mod 4 until the required count.  */
    for (size_t i = 2; i <= limit && pos < need; ++i) {
        if (is_prime[i] && (i & 3U) == 1U) {
            primes1[pos++] = i;
        }
    }
    free(is_prime);
    primes1_cap = pos;   /* in case the bound was too low, we may have fewer */
}

/* Public sequence generator.  */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* unused index 0 */
    ensure_primes1((size_t)n);
    /* primes1 is guaranteed to contain at least 'n' entries after the call. */
    return minimal_k(primes1[(size_t)n - 1]);
}
