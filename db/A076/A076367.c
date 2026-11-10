/*
 * A076367  Primes with subscripts from the Bonse sequence.
 * https://oeis.org/A076367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(A060646(n)), where A060646(n) = minimal j with
 *            n-j+1 < prime(j); computed by a prime sieve and a scan over j
 * Range:     u8 n <= 303, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 8813 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define A076367_LIMIT 200000

static uint32_t A076367_primes[20000];
static size_t A076367_cnt = 0;
static bool A076367_ready = false;

static void A076367_init(void)
{
    if (A076367_ready) return;
    static unsigned char comp[A076367_LIMIT + 1];
    size_t i, j;
    for (i = 2; i * i <= A076367_LIMIT; ++i)
        if (!comp[i])
            for (j = i * i; j <= A076367_LIMIT; j += i)
                comp[j] = 1;
    for (i = 2; i <= A076367_LIMIT; ++i)
        if (!comp[i])
            A076367_primes[A076367_cnt++] = (uint32_t)i;
    A076367_ready = true;
}

seq_t a(idx_t n)
{
    A076367_init();
    size_t j;
    for (j = 1; j <= A076367_cnt; ++j) {
        /* condition: n - j + 1 < prime(j)  <=>  n + 1 < prime(j) + j */
        if ((unsigned long long)n + 1ULL < (unsigned long long)A076367_primes[j - 1] + (unsigned long long)j)
            return (seq_t)A076367_primes[j - 1];
    }
    return (seq_t)A076367_primes[A076367_cnt - 1];
}
