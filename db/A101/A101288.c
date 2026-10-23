/*
 * A101288  The number of primes between the n-th isolated prime and n-th
 *          isolated composite.
 * https://oeis.org/A101288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: number of primes between the n‑th isolated prime and the
 * n‑th isolated composite.
 * The function enumerates all primes up to LIMIT, builds lists of
 * isolated primes (A007510) and isolated composites (numbers whose
 * neighbours are both primes, the averages of twin primes, A014574),
 * then counts the primes strictly between the n‑th elements of those
 * lists.  The implementation aims to be fast for n up to about 10 000
 * and works for any seq_t/idx_t combination that fits in the build. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define LIMIT 4000000u   /* large enough for n ≤ 10 000 */

static bool   *is_prime  = NULL;
static uint32_t *pi_arr   = NULL;
static uint32_t *iso_pr   = NULL;
static uint32_t *iso_comp = NULL;
static size_t   prime_cnt = 0, comp_cnt = 0;
static bool     initialized = false;

static void init(void)
{
    if (initialized) return;

    is_prime  = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    pi_arr    = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    iso_pr    = (uint32_t *)malloc(LIMIT * sizeof(uint32_t));
    iso_comp  = (uint32_t *)malloc(LIMIT * sizeof(uint32_t));

    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    pi_arr[0] = 0;
    for (size_t i = 1; i <= LIMIT; ++i)
        pi_arr[i] = pi_arr[i-1] + (is_prime[i] ? 1u : 0u);

    prime_cnt = 0;
    for (size_t p = 2; p <= LIMIT; ++p) {
        if (!is_prime[p]) continue;
        if (p == 2) {
            iso_pr[prime_cnt++] = (uint32_t)p;
        } else {
            bool left = (p >= 3) ? is_prime[p-2] : false;
            bool right = (p + 2 <= LIMIT) ? is_prime[p+2] : false;
            if (!left && !right)
                iso_pr[prime_cnt++] = (uint32_t)p;
        }
    }

    comp_cnt = 0;
    for (size_t i = 4; i <= LIMIT-1; ++i) {
        if (is_prime[i]) continue;  /* must be composite */
        if (is_prime[i-1] && is_prime[i+1])
            iso_comp[comp_cnt++] = (uint32_t)i;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* defined only for n ≥ 1 */

    init();

    size_t idx = (size_t)n - 1;
    if (idx >= prime_cnt || idx >= comp_cnt)
        return 0;   /* insufficient precomputation, should not occur */

    uint32_t p = iso_pr[idx];
    uint32_t c = iso_comp[idx];
    uint32_t lo = (p < c) ? p : c;
    uint32_t hi = (p > c) ? p : c;

    if (hi <= lo + 1) return 0;     /* no space between them */

    seq_t count = (seq_t)pi_arr[hi-1] - (seq_t)pi_arr[lo];
    return count;
}
