/*
 * A089822  Number of subsets of {1,.., n} containing exactly two primes.
 * https://oeis.org/A089822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(pi(n),2) * 2^(n - pi(n)), where pi(n) = #primes ≤ n
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 37, u64 n <= 77, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If pi(n) < 2, a(n) = 0 */

#include <stdlib.h>
#include <string.h>

static idx_t max_precalc = 0;
static unsigned char *prime_flag = NULL;      /* 1 = prime, 0 = composite */
static uint32_t *pi_prefix = NULL;            /* pi_prefix[i] = #primes ≤ i */

/* Ensure that prime_flag and pi_prefix are computed up to at least n */
static void ensure_prime_prefix(idx_t n)
{
    if (n <= max_precalc) return;

    size_t new_size = (size_t)(n + 1);
    unsigned char *new_flags = (unsigned char *)realloc(prime_flag, new_size);
    if (!new_flags) return;              /* out of memory: result will be 0 */
    /* Assume new bytes are uninitialized; set all to 1 */
    memset(new_flags, 1, new_size);
    new_flags[0] = 0;     /* 0 is not prime */
    if (new_size > 1) new_flags[1] = 0; /* 1 is not prime */

    /* Sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= n; ++p) {
        if (new_flags[p]) {
            for (idx_t m = p * p; m <= n; m += p) {
                new_flags[m] = 0;
            }
        }
    }
    prime_flag = new_flags;

    /* Build prefix array */
    uint32_t *new_pi = (uint32_t *)realloc(pi_prefix, new_size * sizeof(uint32_t));
    if (!new_pi) return;
    new_pi[0] = 0;
    for (idx_t i = 1; i <= n; ++i) {
        new_pi[i] = new_pi[i - 1] + (prime_flag[i] ? 1 : 0);
    }
    pi_prefix = new_pi;
    max_precalc = n;
}

seq_t a(idx_t n)
{
    ensure_prime_prefix(n);
    if (max_precalc < n) return (seq_t)0;   /* failed allocation */

    uint32_t pi = pi_prefix[n];
    if (pi < 2) return (seq_t)0;

    idx_t exp = n - pi;
    seq_t pow2 = (seq_t)1 << exp;               /* 2^(n-pi) */
    seq_t comb = (seq_t)pi * (seq_t)(pi - 1) / (seq_t)2;  /* C(pi,2) */
    return comb * pow2;
}
