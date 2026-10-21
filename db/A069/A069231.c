/*
 * A069231  Numbers n such that there are exactly 3 primes p satisfying the
 *          inequality n < p < n + tau(n)^2 where tau(n) = A000005(n).
 * https://oeis.org/A069231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th integer n such that exactly 3 primes p satisfy n
 *            < p < n + tau(n)^2, where tau(n) = number of divisors of n
 * Range:     u8 n <= 33, u16 n <= 3250, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- globals ---------- */
static uint8_t  *is_composite   = NULL;   /* 0 = possible prime, 1 = composite   */
static uint32_t *spf            = NULL;   /* smallest prime factor of i           */
static uint32_t *tau            = NULL;   /* divisor count tau(i)                 */
static uint32_t *exp_of_p       = NULL;   /* exponent of spf(i) in i              */
static uint32_t *pi             = NULL;   /* prime counting pi(i) = #primes ≤ i   */

static seq_t   *cache          = NULL;   /* array of computed a(n) values        */
static size_t   cache_cap      = 0;       /* capacity of cache                    */
static size_t   cache_len      = 0;       /* number of cached values              */
static size_t   last_checked   = 2;       /* next n to be examined                */

/* ---------- precomputation: sieve & tau computation ---------- */
#define LIMIT 2000000UL   /* sufficient for the first 10 000 terms */

static void precompute(void)
{
    if (is_composite) return;   /* already done */

    size_t N = LIMIT;
    is_composite = (uint8_t *)calloc(N + 1, sizeof(uint8_t));
    spf          = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    tau          = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    exp_of_p     = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    pi           = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    if (!is_composite || !spf || !tau || !exp_of_p || !pi) return; /* OOM */

    size_t max_primes = N / 10 + 1;
    uint32_t *primes = (uint32_t *)malloc(max_primes * sizeof(uint32_t));
    size_t primeCnt = 0;

    tau[1] = 1;
    exp_of_p[1] = 0;
    spf[1] = 1;
    pi[0] = 0;

    for (uint32_t i = 2; i <= N; ++i) {
        if (!is_composite[i]) {
            primes[primeCnt++] = i;
            spf[i]         = i;
            exp_of_p[i]    = 1;
            tau[i]         = 2;            /* prime: two divisors */
        }
        for (size_t j = 0; j < primeCnt; ++j) {
            uint32_t p   = primes[j];
            uint64_t v64 = (uint64_t)i * p;
            if (v64 > N) break;
            uint32_t ip = (uint32_t)v64;
            is_composite[ip] = 1;
            spf[ip] = p;

            if (i % p == 0) {              /* p divides i */
                exp_of_p[ip] = exp_of_p[i] + 1;
                uint32_t e_old = exp_of_p[i];
                uint32_t e_new = exp_of_p[ip];
                tau[ip] = tau[i] / (e_old + 1) * (e_new + 1);
                break;
            } else {                      /* p does not divide i */
                exp_of_p[ip] = 1;
                tau[ip] = tau[i] * 2;
            }
        }
    }

    for (uint32_t i = 1; i <= N; ++i) {
        pi[i] = pi[i-1] + (!is_composite[i] && i >= 2);
    }
    free(primes);
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0, guard */
    size_t target = (size_t)n;

    precompute();

    /* generate terms until we have at least the requested index */
    while (cache_len < target) {
        for (size_t cur = last_checked; cur <= LIMIT; ++cur) {
            uint32_t d   = tau[cur];
            uint64_t L   = (uint64_t)d * (uint64_t)d;   /* window length   */
            size_t high  = cur + (size_t)L - 1;
            if (high > LIMIT) high = LIMIT;            /* safety clamp     */

            uint32_t primeCount = pi[(uint32_t)high] - pi[(uint32_t)cur];
            if (primeCount == 3) {
                /* ensure capacity */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;           /* OOM */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)cur;
                if (cache_len == target) {
                    last_checked = cur + 1;
                    return cache[cache_len - 1];
                }
            }
        }
        /* should not happen with LIMIT=2,000,000 for n<=10,000 */
        return (seq_t)0;
    }

    return cache[target-1];
}
