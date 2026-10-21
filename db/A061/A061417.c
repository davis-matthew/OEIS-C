/*
 * A061417  Number of permutations up to cyclic rotations; permutation
 *          siteswap necklaces.
 * https://oeis.org/A061417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (1/n) * Σ_{d|n} φ(n/d) * (n/d)^d * d!
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/* Prime sieve (up to the largest n requested).               */
/* ------------------------------------------------------------ */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t    max_prime_limit = 0;

static void sieve_primes_upto(idx_t limit)
{
    if (limit <= max_prime_limit) return;
    bool *is_prime = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    for (idx_t i = 2; i <= limit; ++i) is_prime[(size_t)i] = true;
    for (idx_t i = 2; (idx_t)((uint64_t)i * i) <= limit; ++i)
        if (is_prime[(size_t)i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[(size_t)j] = false;
        }
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i]) cnt++;
    prime_list = (uint64_t *)realloc(prime_list, cnt * sizeof(uint64_t));
    prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i]) prime_list[prime_count++] = (uint64_t)i;
    free(is_prime);
    max_prime_limit = limit;
}

/* ------------------------------------------------------------ */
/* Euler's totient – uses pre‑computed primes.                 */
/* ------------------------------------------------------------ */
static seq_t phi(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t temp = m;
    for (size_t i = 0; i < prime_count && (idx_t)prime_list[i] * (idx_t)prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            result -= result / (seq_t)p;
        }
    }
    if (temp > 1) result -= result / (seq_t)temp;
    return result;
}

/* ------------------------------------------------------------ */
/* Simple multiplication (no overflow checks – relies on seq_t */
/* being big enough or arbitrary‑precision).                     */
/* ------------------------------------------------------------ */
static inline seq_t mul_seq(seq_t a, seq_t b)
{
    return a * b;
}

/* ------------------------------------------------------------ */
/* Generic binary exponentiation.                              */
/* ------------------------------------------------------------ */
static seq_t pow_seq_custom(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result = mul_seq(result, base);
        exp >>= 1;
        if (exp) base = mul_seq(base, base);
    }
    return result;
}

/* ------------------------------------------------------------ */
/* Factorial cache: fact_cache[i] = i!  (0! = 1).              */
/* ------------------------------------------------------------ */
static seq_t *fact_cache = NULL;   /* fact_cache[i] = i!  */
static size_t fact_cap   = 0;
static idx_t  fact_size  = 0;

static void ensure_factorial_up_to(idx_t n)
{
    if (n <= fact_size) return;
    size_t new_cap = fact_cap ?: 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
    fact_cache = tmp;
    fact_cap = new_cap;
    if (fact_size == 0) fact_cache[0] = (seq_t)1;
    for (idx_t i = fact_size + 1; i <= n; ++i) {
        fact_cache[i] = mul_seq(fact_cache[i - 1], (seq_t)i);
    }
    fact_size = n;
}

/* ------------------------------------------------------------ */
/* Main sequence function.                                      */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* offset starts at 1 */
    sieve_primes_upto(n);
    ensure_factorial_up_to(n);          /* need up to (n‑1)! */

    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            idx_t m = n / d;            /* m = n/d */
            seq_t term = phi(m);
            term = mul_seq(term, pow_seq_custom((seq_t)m, d - 1));
            term = mul_seq(term, fact_cache[d - 1]); /* (d-1)! */
            sum = sum + term;

            /* paired divisor d2 = n/d, if distinct */
            idx_t d2 = n / d;
            if (d2 != d) {
                m = n / d2;             /* m = d */
                term = phi(m);
                term = mul_seq(term, pow_seq_custom((seq_t)m, d2 - 1));
                term = mul_seq(term, fact_cache[d2 - 1]); /* (d2-1)! */
                sum = sum + term;
            }
        }
    }
    return sum;
}
