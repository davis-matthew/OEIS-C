/*
 * A050320  Number of ways n is a product of squarefree numbers > 1.
 * https://oeis.org/A050320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = number of ways n is a product of squarefree numbers > 1
 *            (non‑decreasing factorizations into squarefree factors)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* -------------------------------------------------------------
   Prime sieve – generate all primes up to 10000 (enough for n ≤ 10000).
   ------------------------------------------------------------- */
static const idx_t MAX_PRIME = 10000;
static idx_t primes[1230];
static size_t prime_cnt = 0;
static bool primes_built = false;

static void build_primes(void)
{
    bool is_prime[MAX_PRIME + 1];
    for (size_t i = 0; i <= MAX_PRIME; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= MAX_PRIME; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= MAX_PRIME; j += i)
                is_prime[j] = false;
    for (size_t i = 2; i <= MAX_PRIME; ++i)
        if (is_prime[i]) primes[prime_cnt++] = (idx_t)i;
    primes_built = true;
}

/* -------------------------------------------------------------
   Factor n into its distinct prime factors (ignoring multiplicities).
   ------------------------------------------------------------- */
static void factor_distinct(idx_t n, idx_t *pf, size_t *cnt)
{
    *cnt = 0;
    idx_t temp = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            pf[(*cnt)++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) pf[(*cnt)++] = temp;   /* remaining prime > sqrt(n) */
}

/* -------------------------------------------------------------
   Recursive computation of a(n) with non‑decreasing factors.
   ------------------------------------------------------------- */
static seq_t rec(idx_t n, idx_t min_factor)
{
    if (n == 1) return (seq_t)1;          /* finished a factorization */
    if (min_factor > n) return (seq_t)0;  /* impossible to continue */

    idx_t pf[20];   /* at most ~10 distinct primes for n ≤ 10000 */
    size_t cnt;
    factor_distinct(n, pf, &cnt);

    seq_t result = (seq_t)0;

    /* Enumerate all non‑empty subsets of the distinct primes – each subset
       corresponds to a squarefree divisor of n. */
    if (cnt == 0) return result;          /* should not happen for n>1 */

    size_t total_masks = ((size_t)1 << cnt);
    for (size_t mask = 1; mask < total_masks; ++mask) {
        idx_t prod = (idx_t)1;
        for (size_t i = 0; i < cnt; ++i)
            if (mask & ((size_t)1 << i))
                prod *= pf[i];

        if (prod < min_factor || prod > n) continue;
        result += rec(n / prod, prod);  /* enforce non‑decreasing order */
    }
    return result;
}

/* -------------------------------------------------------------
   Public function required by the build system.
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!primes_built) build_primes();
    return rec(n, (idx_t)2);
}
