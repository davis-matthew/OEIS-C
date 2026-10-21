/*
 * A094959  Number of positive integer coefficients in n-th Bernoulli
 *          polynomial.
 * https://oeis.org/A094959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (non‑zero) integer coefficients in the
 *            n-th Bernoulli polynomial B_n(x). The coefficient of x^(n−k) is
 *            binom(n,k)*B_k. For even k>0 the Bernoulli number B_k has
 *            reduced denominator D(k)=Π{primes p : p−1 | k}. The coefficient
 *            is an integer ⇔ D(k) divides binom(n,k). For k=1,...
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   divisible_by_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------- include needed headers ---------- */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/divisible_by_prime.h"

/* ---------- static data for primes ---------- */
static uint32_t *PRIMES = NULL;        /* list of primes up to current_limit+1 */
static size_t   PRIMES_COUNT = 0;      /* number of primes stored */
static idx_t    current_limit = 0;     /* largest value for which primes are valid */

/* ---------- helper to generate primes up to max_val (inclusive) ---------- */
static void init_primes_up_to(idx_t max_val)
{
    if (PRIMES && max_val <= current_limit)
        return;            /* already have enough primes */

    /* allocate and clear sieve */
    bool *sieve = (bool*)calloc((size_t)(max_val + 1), sizeof(bool));
    if (!sieve)
        exit(EXIT_FAILURE);
    for (size_t i = 0; i <= (size_t)max_val; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (idx_t p = 2; p * p <= max_val; ++p)
        if (sieve[p])
            for (idx_t q = p * p; q <= max_val; q += p)
                sieve[q] = false;

    /* count primes and store them */
    size_t cnt = 0;
    for (idx_t i = 2; i <= max_val; ++i)
        if (sieve[i]) ++cnt;

    uint32_t *new_primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!new_primes)
        exit(EXIT_FAILURE);

    size_t idx = 0;
    for (idx_t i = 2; i <= max_val; ++i)
        if (sieve[i]) new_primes[idx++] = (uint32_t)i;

    /* replace old data */
    free(PRIMES);
    PRIMES = new_primes;
    PRIMES_COUNT = cnt;
    current_limit = max_val;

    free(sieve);
}

/* ---------- the required sequence function ---------- */
seq_t a(idx_t n)
{
    /* need primes up to n+1 for the denominator test */
    init_primes_up_to(n + 1);

    seq_t count = 1;                 /* coefficient of x^n (k=0) is always 1 */

    for (idx_t k = 1; k <= n; ++k) {
        if (k == 1) {                /* B1 = –½, denominator 2 */
            if (n % 2 == 0)
                ++count;
            continue;
        }
        if (k & 1)                   /* odd k>1 gives zero coefficient */
            continue;

        /* even k ≥ 2 : test divisibility by all primes p with (p‑1) | k */
        bool integer = true;
        for (size_t i = 0; i < PRIMES_COUNT; ++i) {
            uint32_t p = PRIMES[i];
            if (p > k + 1)          /* denominators only involve primes ≤ k+1 */
                break;
            if ((k % (p - 1)) != 0) /* prime not in the denominator of B_k */
                continue;
            if (!divisible_by_prime(n, k, p)) {
                integer = false;
                break;
            }
        }
        if (integer)
            ++count;
    }
    return count;
}
