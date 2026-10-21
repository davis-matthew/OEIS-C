/*
 * A096501  Difference between primes preceding n+1 and n.
 * https://oeis.org/A096501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prev_prime(n+1) - prev_prime(n) with prev_prime(1) =
 *            prev_prime(2) = -2 for n = 1,2
 * Range:     u8 n <= 8992, u16 n <= 8992, u32 n <= 8992, u64 n <= 8992,
 *            u128 n <= 8992, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint8_t *sieve_arr = NULL;   /* 1 = prime candidate, 0 = composite   */
static idx_t sieve_limit = 0;       /* largest index currently covered    */

/* extend the sieve up to `limit` (inclusive) */
static void extend_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    uint8_t *new_arr = (uint8_t *)realloc(sieve_arr,
                                          (limit + 1) * sizeof(uint8_t));
    if (!new_arr)
        abort();
    sieve_arr = new_arr;

    for (idx_t i = sieve_limit + 1; i <= limit; ++i)
        sieve_arr[i] = 1;            /* assume prime */

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (sieve_arr[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                sieve_arr[j] = 0;   /* mark composites */
        }
    }

    sieve_limit = limit;
}

/* largest prime strictly less than `m`; for m ≤ 3 returns 2 (used only
   for the hand‑crafted base cases) */
static idx_t prev_prime_idx(idx_t m)
{
    if (m <= 3)
        return 2;                  /* covers m = 3: prime 2 */

    if (m - 1 > sieve_limit)
        extend_sieve(m - 1);

    for (idx_t k = m - 1; k >= 2; --k) {
        if (sieve_arr[k])
            return k;
    }
    return 2;                      /* unreachable for m ≥ 4 */
}

/* OEIS sequence A096501 */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)4;

    idx_t p1 = prev_prime_idx(n + 1);
    idx_t p2 = prev_prime_idx(n);

    return (seq_t)(p1 - p2);
}
