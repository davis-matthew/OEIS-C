/*
 * A020940  Greatest k such that (k-th prime) < (10 times n-th prime).
 * https://oeis.org/A020940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = maximum k such that prime(k) < 10 × prime(n)
 * Range:     u8 n <= 37, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* -------------------------------------------------------------------- */
static void compute_primes_upto(uint64_t limit, uint64_t **primes_out,
                                size_t *cnt_out)
{
    /* Sieve: use a byte per number (0 = prime candidate, 1 = composite). */
    uint8_t *is_comp = (uint8_t *)calloc((size_t)limit + 1, 1);
    if (!is_comp) { *primes_out = NULL; *cnt_out = 0; return; }

    /* Do the classic sieve of Eratosthenes. */
    uint64_t i, j;
    for (i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* First pass: count primes. */
    size_t cnt = 0;
    for (i = 2; i <= limit; ++i) if (!is_comp[i]) ++cnt;

    /* Allocate array for primes. */
    uint64_t *primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) { free(is_comp); *primes_out = NULL; *cnt_out = 0; return; }

    /* Second pass: store primes. */
    size_t idx = 0;
    for (i = 2; i <= limit; ++i) if (!is_comp[i]) primes[idx++] = i;

    free(is_comp);
    *primes_out = primes;
    *cnt_out    = cnt;
}

/* -------------------------------------------------------------------- */
/*  Local primitive to generate all primes up to a given limit.         */

/* -------------------------------------------------------------------- */
/*  Static storage for the primes list.                                 */
/* -------------------------------------------------------------------- */
static uint64_t *global_primes   = NULL;
static size_t   global_prime_cnt = 0;
static const uint64_t SIEVE_LIMIT = 5000000ULL;   /* enough for n <= 10000 */

/* -------------------------------------------------------------------- */
/*  Public sequence function.                                           */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure primes table is built only once. */
    if (global_primes == NULL) {
        compute_primes_upto(SIEVE_LIMIT, &global_primes, &global_prime_cnt);
    }

    /* Undefined behaviour is allowed for n < 1 (offset). */
    if (n == 0 || n > global_prime_cnt) return 0;

    /* Get the n‑th prime (array index is zero‑based). */
    uint64_t p_n = global_primes[(size_t)n - 1];

    /* We want largest k with prime(k) < 10 * p_n. */
    uint64_t bound = p_n * 10U - 1U;  /* < 10 × p_n */

    /* Binary search in the primes array for the count of primes ≤ bound. */
    size_t lo = 0, hi = global_prime_cnt;
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if (global_primes[mid] <= bound)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo is the number of primes strictly less than 10 × p_n. */
    return (seq_t)lo;
}
