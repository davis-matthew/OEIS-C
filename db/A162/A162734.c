/*
 * A162734  An alternating sum of all numbers from the n-th up to the (n+1)st
 *          isolated prime.
 * https://oeis.org/A162734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = sum_{j=A007510(n)..A007510(n+1)} (-1)^(j+1)*j * =
 *            A001057(A007510(n+1)) - A001057(A007510(n)-1)
 * Range:     u8 n <= 21, u16 n <= 4823, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   altSum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/altSum.h"

/* --- configuration ----------------------------------------------------- */
static const idx_t LIMIT = 400000;      /* sufficient for n up to ~10 000 */

/* --- internal state ----------------------------------------------------- */
static uint64_t *isolated   = NULL;   /* list of isolated primes (A007510) */
static size_t   isolated_cnt = 0;      /* number of stored isolated primes */
static bool     initialized = false;   /* indicates if the list is built */

/* --- build isolated primes once ----------------------------------------- */
static void build_isolated_primes(void)
{
    /* Breadth‑first sieve up to LIMIT */
    bool *prime = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!prime) return;
    for (idx_t i = 2; i <= LIMIT; ++i) prime[i] = true;
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                prime[j] = false;
        }
    }

    /* Count isolated primes */
    size_t cnt = 0;
    for (idx_t p = 2; p <= LIMIT; ++p) {
        if (!prime[p]) continue;
        bool left  = (p > 2)  && prime[p - 2];
        bool right = (p + 2 <= LIMIT) && prime[p + 2];
        if (!left && !right) ++cnt;
    }

    isolated = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!isolated) {
        free(prime);
        return;
    }
    isolated_cnt = cnt;

    /* Fill isolated array */
    size_t idx = 0;
    for (idx_t p = 2; p <= LIMIT; ++p) {
        if (!prime[p]) continue;
        bool left  = (p > 2)  && prime[p - 2];
        bool right = (p + 2 <= LIMIT) && prime[p + 2];
        if (!left && !right) isolated[idx++] = (uint64_t)p;
    }

    free(prime);
    initialized = true;
}

/* --- sequence -----------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1 */

    if (!initialized) build_isolated_primes();
    if (!initialized || isolated_cnt <= (size_t)n) return 0;  /* insufficient data */

    uint64_t p_n     = isolated[n - 1];      /* A007510(n)   */
    uint64_t p_next  = isolated[n];          /* A007510(n+1) */

    long long sum = altSum(p_next) - altSum(p_n - 1);
    return (seq_t)sum;
}
