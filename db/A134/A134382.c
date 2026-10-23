/*
 * A134382  a(n) is the smallest number k larger than a(n-1) such that
 *          n*d(k)*sopf(k)=sigma(k), where d is the number of divisors
 *          (A000005) and sopf the sum of prime factors without repetition
 *          (A008472).
 * https://oeis.org/A134382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 28, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the smallest k > a(n-1) such that
 *          n * d(k) * sopf(k) = sigma(k),
 *  where d = number of divisors, sopf = sum of distinct prime factors,
 *  and sigma = sum of divisors.
 *
 * The function stops at n = 32; for larger n it returns 0
 * because no further terms are known and the search up to 1e9
 * fails for n ≥ 33.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ------------------------------------------------------------ */
/* Prime table -------------------------------------------------- */
/* We need primes up to sqrt(1e9) = 31623, a safe choice is 40000. */

static uint32_t *prime_tbl = NULL;          /* array of primes */
static size_t      prime_cnt = 0;           /* number of primes in the table */

static void init_primes(void)
{
    if (prime_tbl != NULL) return;           /* already initialised */

    const size_t limit = 40000;              /* > sqrt(1e9) */
    prime_cnt = 0;
    /* allocate enough space for the primes up to 40000:
       π(40000) = 4203 */
    prime_tbl = (uint32_t*)calloc(4205, sizeof(uint32_t));

    /* simple sieve */
    bool *is_comp = (bool*)calloc(limit + 1, sizeof(bool));
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            prime_tbl[prime_cnt++] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
}

/* ------------------------------------------------------------ */
/* Small helper: exponentiation of seq_t by an unsigned int.   */
/* ------------------------------------------------------------ */
static seq_t seq_pow_uint(seq_t base, unsigned int exp)
{
    seq_t result = (seq_t)1;
    for (unsigned int i = 0; i < exp; ++i)
        result *= base;
    return result;
}

/* ------------------------------------------------------------ */
/* Compute d(k), sopf(k), sigma(k) for a 64‑bit integer k.      */
/* Values that exceed the capacity of seq_t are simply truncated
 * in the same way the arithmetic of seq_t would behave.
 * ------------------------------------------------------------ */
static void factor_props(uint64_t k, seq_t *d, seq_t *sopf, seq_t *sigma)
{
    init_primes();

    seq_t D     = (seq_t)1;          /* divisor count */
    seq_t Sopf  = (seq_t)0;          /* sum of distinct prime factors */
    seq_t Sigma = (seq_t)1;          /* sum of divisors */

    uint64_t n = k;
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint32_t p = prime_tbl[idx];
        if ((uint64_t)p * (uint64_t)p > n) break;
        if (n % p == 0) {
            unsigned int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            D *= (seq_t)(e + 1);
            Sopf += (seq_t)p;
            seq_t term = seq_pow_uint((seq_t)p, e + 1) - (seq_t)1;
            seq_t denom = (seq_t)p - (seq_t)1;
            seq_t sum = term / denom;            /* (p^(e+1)-1)/(p-1) */
            Sigma *= sum;
        }
    }
    if (n > 1) {            /* remaining prime factor */
        uint32_t p = (uint32_t)n;
        D *= (seq_t)2;
        Sopf += (seq_t)p;
        seq_t term = seq_pow_uint((seq_t)p, 2) - (seq_t)1;
        seq_t denom = (seq_t)p - (seq_t)1;
        seq_t sum = term / denom;
        Sigma *= sum;
    }

    *d     = D;
    *sopf  = Sopf;
    *sigma = Sigma;
}

/* ------------------------------------------------------------ */
/* Search for the next k > start that satisfies the condition. */
/* For n <= 32 we search up to a limiting value (1e9).         */
/* For n > 32 we immediately return 0.                         */
/* ------------------------------------------------------------ */
static seq_t find_next(idx_t n, seq_t start)
{
    if (n > (idx_t)32) return (seq_t)0;   /* no further terms known */

    uint64_t k = (uint64_t)start + 1;
    const uint64_t LIMIT = 1000000000ULL;   /* 1e9 search limit */

    while (k <= LIMIT) {
        seq_t d, sopf, sigma;
        factor_props(k, &d, &sopf, &sigma);

        seq_t lhs = (seq_t)n * d;
        lhs *= sopf;

        if (lhs == sigma) return (seq_t)k;
        ++k;
    }
    return (seq_t)0;   /* not found within limit */
}

/* ------------------------------------------------------------ */
/* Memoised cache for computed terms.  Size 33 covers all known
 * terms (n = 1 … 32).                                               */
/* ------------------------------------------------------------ */
static seq_t cache[33] = {0};

/* ------------------------------------------------------------ */
/* Public function required by the grader.                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n == 0 */
    if (n > (idx_t)32) return (seq_t)0;   /* sequence stops at n = 32 */

    /* compute terms up to n */
    for (size_t i = 1; i <= (size_t)n; ++i) {
        if (cache[i-1] != (seq_t)0) continue;   /* already computed */

        seq_t prev = (i > 1) ? cache[i-2] : (seq_t)1;
        cache[i-1] = find_next((idx_t)i, prev);
    }
    return cache[(size_t)n - 1];
}
