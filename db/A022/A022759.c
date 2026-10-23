/*
 * A022759  n-th 6k+1 prime plus n-th 6k+5 prime.
 * https://oeis.org/A022759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002476(n) + A007528(n) (n‑th prime ≡ 1 (mod 6)) + (n‑th
 *            prime ≡ 5 (mod 6))
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes_1 = NULL;
static size_t count1 = 0;
static uint64_t *primes_5 = NULL;
static size_t count5 = 0;
static idx_t current_limit = 0;

/* Ensure we have at least n primes of each required form. */
static void ensure_primes(idx_t n)
{
    if (count1 >= n && count5 >= n)
        return;

    /* Start with a reasonable estimate for the limit. */
    idx_t limit = (current_limit == 0) ? (n * 20 + 10) : current_limit;
    if (limit < n * 20 + 10)
        limit = n * 20 + 10;

    while (1) {
        uint64_t *all_primes = NULL;
        size_t total_cnt = 0;

        /* build_primes_upto generates all primes ≤ limit. */
        build_primes_upto(limit, &all_primes, &total_cnt);

        /* Reallocate storage for the filtered primes. */
        free(primes_1);
        free(primes_5);
        primes_1 = NULL;
        primes_5 = NULL;
        count1 = 0;
        count5 = 0;

        primes_1 = (uint64_t *)malloc(total_cnt * sizeof(uint64_t));
        primes_5 = (uint64_t *)malloc(total_cnt * sizeof(uint64_t));

        /* Split the primes into the two required congruence classes. */
        for (size_t i = 0; i < total_cnt; ++i) {
            uint64_t p = all_primes[i];
            if (p % 6 == 1) {
                primes_1[count1++] = p;
            } else if (p % 6 == 5) {
                primes_5[count5++] = p;
            }
        }

        free(all_primes);
        current_limit = limit;

        if (count1 >= n && count5 >= n)
            break;

        /* Increase the limit and try again. */
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1, but guard anyway */
        return 0;
    ensure_primes(n);
    /* Convert indices to size_t for array indexing. */
    size_t idx = (size_t)(n - 1);
    seq_t p1 = (seq_t)primes_1[idx];
    seq_t p5 = (seq_t)primes_5[idx];
    return p1 + p5;
}
