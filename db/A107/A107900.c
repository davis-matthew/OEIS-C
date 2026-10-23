/*
 * A107900  Sum of terms of continued fraction for prime(n)/n.
 * https://oeis.org/A107900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 439, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = sum of partial quotients of the continued fraction of prime(n)/n */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/simple_sieve.h"

/* Static cache of the first N primes */
static uint32_t *prime_arr = NULL;
static size_t prime_cnt   = 0;

/* Return the nth term of the sequence (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n < 1 */
        return 0;

    /* Ensure we have at least n primes cached */
    if ((size_t)n > prime_cnt) {
        /* Rough upper bound for the nth prime (safe for n up to ~200k) */
        size_t limit = (size_t)(n * 20);
        if (limit < 100) limit = 100;

        size_t cnt;
        uint32_t *arr = NULL;

        simple_sieve((uint32_t)limit, &arr, &cnt);

        /* If bound was too low, double until enough primes */
        while (cnt < n) {
            limit *= 2;
            free(arr);
            simple_sieve((uint32_t)limit, &arr, &cnt);
        }

        /* Replace the old cache */
        free(prime_arr);
        prime_arr = arr;
        prime_cnt = cnt;
    }

    /* nth prime (1‑based index) */
    uint32_t p = prime_arr[n - 1];

    /* Compute sum of continued‑fraction partial quotients of p/n */
    idx_t a_val = (idx_t)p;
    idx_t b_val = n;
    idx_t sum   = 0;

    while (b_val != 0) {
        idx_t q = a_val / b_val;
        sum += q;
        idx_t r = a_val % b_val;
        a_val = b_val;
        b_val = r;
    }

    return (seq_t)sum;
}
