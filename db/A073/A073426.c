/*
 * A073426  Number of primes between n-th prime and n-th composite or number
 *          of primes that are >=Min[prime(n),c(n)] and are <
 *          Max[prime(n),c(n)].
 * https://oeis.org/A073426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = n - π(A002808(n+1))          for n ≥ 5
 *           a(n) = π(A002808(n+1)) - n + 1   for n < 5
 * where π(x) is the prime‑counting function and
 * A002808 is the composite‑number sequence
 * (1, 4, 6, 8, 9, 10, 12, 14, …). 
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Prime‑counting data structures – lazily initialised on first call */
struct primes_data {
    bool  *is_prime;   /* 1 if prime, 0 otherwise */
    int   *pi;         /* π(n) – number of primes ≤ n */
    int   *comp;       /* A002808 sequence: comp[0]=1, comp[1]=4, … */
    size_t comp_len;   /* number of elements stored in comp (including the 1) */
};

static struct primes_data pd = {0};

static void init_primes(void)
{
    const idx_t LIMIT = 200000;          /* enough for n up to ≈10⁵ */
    const idx_t P = LIMIT + 1;

    /* Allocate memory */
    pd.is_prime = (bool*)malloc(P * sizeof(bool));
    pd.pi       = (int  *)malloc(P * sizeof(int));
    pd.comp     = NULL;                 /* will be allocated later */
    if (!pd.is_prime || !pd.pi) return;

    /* Initialise all as prime, then correct for 0 and 1 */
    memset(pd.is_prime, 1, P * sizeof(bool));
    pd.is_prime[0] = pd.is_prime[1] = 0;

    /* Classic sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= LIMIT; ++p)
        if (pd.is_prime[p])
            for (idx_t m = p * p; m <= LIMIT; m += p)
                pd.is_prime[m] = 0;

    /* Build π(n) array */
    pd.pi[0] = 0;
    for (idx_t i = 1; i <= LIMIT; ++i)
        pd.pi[i] = pd.pi[i - 1] + pd.is_prime[i];

    /* Build composite sequence A002808 */
    /* Estimate number of composites ≤ LIMIT to size array */
    size_t est = LIMIT - (pd.pi[LIMIT]) - 1 + 2; /* +2 for the leading 1 */
    pd.comp = (int*)malloc(est * sizeof(int));
    if (!pd.comp) return;
    pd.comp[0] = 1;          /* A002808(1) = 1 */
    pd.comp_len = 1;

    for (idx_t val = 4; val <= LIMIT; ++val) /* start at first composite >1 */
        if (!pd.is_prime[val]) {
            pd.comp[pd.comp_len++] = (int)val;
        }
}

seq_t a(idx_t n)
{
    /* Lazily initialise cache */
    if (!pd.is_prime)
        init_primes();

    /* Safety: if n is larger than we precomputed, return 0 */
    if (n + 1 >= pd.comp_len)
        return 0;

    /* Composite number to be used: A002808(n+1) = comp[n] */
    idx_t c = pd.comp[n];          /* n is 0‑based index for comp */
    idx_t pi_c = pd.pi[c];

    if (n < 5) {
        /* a(n) = π(c) - n + 1   for n = 1,2,3,4 */
        return (seq_t)(pi_c - n + 1);
    } else {
        /* a(n) = n - π(c)       for n ≥ 5 */
        return (seq_t)(n - pi_c);
    }
}
