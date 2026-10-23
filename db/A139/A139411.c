/*
 * A139411  Numbers n such that pi(n)=10^pi(d_1)+10^pi(d_2)+...+10^pi(d_k)
 *          where d_1d_2...d_k is the decimal expansion of n.
 * https://oeis.org/A139411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A139411: numbers n such that pi(n) = Σ 10^pi(d) over the decimal digits d of n.
   The function enumerates all such n up to a fixed limit (1 000 000) and
   returns the nth term (1‑based). If n exceeds the number of solutions found,
   it returns 0. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* mapping digit → π(d) for d = 0 … 9 */
static const uint8_t pi_digit[10] = {
    0, 0,    /* 0, 1 */
    1, 2,    /* 2, 3 */
    2, 3,    /* 4, 5 */
    3, 4,    /* 6, 7 */
    4, 4     /* 8, 9 */
};

/* cache of 10^i for i = 0 … 4 */
static seq_t pow10_cache[5] = {0};

static bool pow10_built = false;
static void build_pow10_cache(void)
{
    if (pow10_built) return;
    pow10_cache[0] = (seq_t)1;
    for (idx_t i = 1; i < 5; ++i)
        pow10_cache[i] = pow10_cache[i - 1] * (seq_t)10;
    pow10_built = true;
}

/* fixed storage for found terms – sufficient for the known list */
#define MAX_TERMS 2000
static seq_t terms[MAX_TERMS];
static size_t term_count = 0;
static bool terms_built = false;

/* Build all solutions up to LIMIT */
static void build_terms(void)
{
    const idx_t LIMIT = 1000000;          /* covers all known solutions */
    /* sieve of Eratosthenes for primes ≤ LIMIT */
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) return;

    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    if (LIMIT >= 0) is_prime[0] = false;
    if (LIMIT >= 1) is_prime[1] = false;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    /* prefix array π(n) */
    uint32_t *pi = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!pi) {
        free(is_prime);
        return;
    }
    uint32_t cum = 0;
    for (idx_t i = 0; i <= LIMIT; ++i) {
        if (is_prime[i]) ++cum;
        pi[i] = cum;
    }

    build_pow10_cache();

    /* enumerate numbers and test the condition */
    for (idx_t n = 2; n <= LIMIT; ++n) {
        seq_t sum = 0;
        idx_t temp = n;
        while (temp > 0) {
            uint8_t d = temp % 10;
            sum += pow10_cache[pi_digit[d]];
            temp /= 10;
        }
        if (pi[n] == (uint32_t)sum) {
            if (term_count < MAX_TERMS) {
                terms[term_count++] = (seq_t)n;
            }
        }
    }

    free(is_prime);
    free(pi);
}

/* Public function required by the host. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* undefined for 0 */
    if (!terms_built) {
        build_terms();
        terms_built = true;
    }
    if (n > term_count) return 0;              /* out of range */
    return terms[n - 1];                        /* 1‑based index */
}
