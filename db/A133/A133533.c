/*
 * A133533  Sum of sixth powers of three consecutive primes.
 * https://oeis.org/A133533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^6 + prime(n+1)^6 + prime(n+2)^6
 * Range:     u8 none, u16 n <= 1, u32 n <= 10, u64 n <= 217, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 9525 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* generate primes up to 'limit' (inclusive) */
static void generate_primes_up_to(idx_t limit, uint32_t **out, size_t *cnt_out)
{
    size_t sz = limit + 1;
    unsigned char *is_composite = (unsigned char *)calloc(sz, 1);
    if (!is_composite) {
        /* Allocation failure – abort program (trusted environment) */
        abort();
    }

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[j] = 1;
        }
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) ++cnt;

    uint32_t *arr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!arr) {
        free(is_composite);
        abort();
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            arr[idx++] = (uint32_t)i;

    free(is_composite);

    *out   = arr;
    *cnt_out = cnt;
}

/* --------------------------------------------------------------------
   Simple sieve of Eratosthenes that stores the primes in a static
   array and expands as needed.  The function generates all primes up
   to 'limit' (inclusive) and stores them in a dynamically allocated
   array.  It frees any previous prime array.
   -------------------------------------------------------------------- */
static uint32_t *primes = NULL;          /* array of primes, 1‑based */
static size_t   primes_count = 0;        /* number of primes stored */

/* Ensure we have at least 'needed' primes (indexing from 1).          */
static void ensure_primes(idx_t needed)
{
    if (needed <= primes_count)
        return;

    /* Rough over‑estimate of the n‑th prime: p_n < n*(20) + 1000 for n ≤ 20000
       (works for our intended n ≤ 10000).  This limits memory usage. */
    idx_t limit = needed * 20 + 1000;
    if (limit < 15)
        limit = 15;

    uint32_t *new_arr;
    size_t    new_cnt;
    generate_primes_up_to(limit, &new_arr, &new_cnt);

    free(primes);
    primes      = new_arr;
    primes_count = new_cnt;
}

/* --------------------------------------------------------------------
   Main sequence function.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)   /* the sequence is defined for n ≥ 1 only */
        return (seq_t)0;

    /* Make sure the prime array contains at least p_{n+2}.              */
    ensure_primes(n + 2);

    seq_t sum = (seq_t)0;
    for (idx_t i = 0; i < 3; ++i) {
        uint32_t p = primes[n - 1 + i];          /* n-1 because primes[0] is p_1 */
        seq_t term = (seq_t)p;

        /* compute p^6   */
        term *= term;          /* p^2 */
        seq_t t2 = term * term;   /* p^4 */
        term    = t2 * term;      /* p^6 */

        sum += term;
    }
    return sum;
}
