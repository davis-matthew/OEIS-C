/*
 * A163059  An alternating sum from 4*n-3 up to the smaller of the n-th twin
 *          primes.
 * https://oeis.org/A163059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001057(A001359(n)) – A001057(4*n-4), where A001057(u) =
 *            (u odd) ? (u+1)/2 : -(u/2)
 * Range:     u8 n <= 22, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   alt_sum_signed, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes the provided primality test helper: is_prime(idx_t). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/alt_sum_signed.h"

/* ----------  Storage for the first element of each twin prime pair  ---------- */
static uint64_t *twin_primes = NULL;   /* array of first elements of twin pairs  */
static size_t   twin_cnt   = 0;        /* number of twin pairs stored so far     */
static size_t   twin_cap   = 0;        /* current allocation size               */
static uint64_t last_twin   = 0;        /* last stored first prime of a twin pair */

/* Ensure that at least n twin primes have been stored.  */
static void ensure_n_twin_primes(idx_t n)
{
    while (twin_cnt < n) {
        /* Start searching after the last found twin prime.  */
        uint64_t start = (twin_cnt == 0) ? 3 : last_twin + 2;
        for (uint64_t p = start; ; p += 2) {
            if (is_prime((idx_t)p) && is_prime((idx_t)(p + 2))) {
                /* Store p as the first prime of a twin pair.  */
                if (twin_cnt == twin_cap) {
                    twin_cap = (twin_cap == 0) ? 16 : twin_cap * 2;
                    uint64_t *tmp = (uint64_t *)realloc(twin_primes, twin_cap * sizeof(uint64_t));
                    if (!tmp) {
                        /* In an error situation, exit the program – the build
                           environment does not provide a custom error handler. */
                        abort();
                    }
                    twin_primes = tmp;
                }
                twin_primes[twin_cnt++] = p;
                last_twin = p;
                break;
            }
        }
    }
}

/* Retrieve the n-th twin prime (1‑based).  */
static idx_t nth_twin_prime(idx_t n)
{
    ensure_n_twin_primes(n);
    return (idx_t)twin_primes[n - 1];
}

/* ----------  Public sequence function  ---------- */
seq_t a(idx_t n)
{
    /* Compute the upper and lower limits for A001057.  */
    idx_t twin = nth_twin_prime(n);            /* A001359(n)   */
    int64_t u = alt_sum_signed(twin);          /* A001057(twin)  */
    int64_t l = alt_sum_signed(4 * n - 4);      /* A001057(4n-4)  */

    /* The result is non‑negative; cast to seq_t.  */
    return (seq_t)(u - l);
}
