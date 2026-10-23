/*
 * A182946  Array of odd prime powers p^j, where j>=1, by antidiagonals.
 * https://oeis.org/A182946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A182946: array of odd prime powers p^j with j ≥ 1, enumerated by antidiagonals. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp); */

/* Static storage for odd primes (3,5,7,…) */
static uint64_t *primes = NULL;      /* 0‑based array of primes */
static size_t   prime_cnt = 0;       /* number of primes stored */
static size_t   prime_cap = 0;       /* allocated capacity */

/* Ensure that the list contains at least 'needed' odd primes. */
static void ensure_primes(size_t needed)
{
    if (needed <= prime_cnt)
        return;   /* already available */

    /* Grow the array capacity if necessary */
    if (prime_cap < needed) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        while (new_cap < needed)
            new_cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        primes = tmp;
        prime_cap = new_cap;
    }

    /* Generate missing primes by simple trial division (odd only). */
    uint64_t cand = (prime_cnt == 0) ? 3 : primes[prime_cnt - 1] + 2;
    while (prime_cnt < needed) {
        bool ok = true;
        for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= cand; ++i) {
            if (cand % primes[i] == 0) {
                ok = false;
                break;
            }
        }
        if (ok)
            primes[prime_cnt++] = cand;
        cand += 2;
    }
}

seq_t a(idx_t n)
{
    /* Find the diagonal index s (s ≥ 2) such that (s-1)*s/2 ≥ n. */
    uint64_t s = 2;
    while (((s - 1) * s) / 2 < (uint64_t)n)
        ++s;

    /* Number of elements before this diagonal. */
    uint64_t prev = ((s - 2) * (s - 1)) / 2;

    /* Position within the diagonal (1‑based). */
    uint64_t k = (uint64_t)n - prev;

    /* Coordinates: i = prime index (1‑based), j = exponent (1‑based). */
    uint64_t i = k;          /* i ranges from 1 to s-1 */
    uint64_t j = s - i;      /* j ranges from 1 to s-1 */

    /* Fetch the i‑th odd prime. */
    ensure_primes(i);
    uint64_t prime = primes[i - 1];   /* array is 0‑based */

    /* Compute prime^j using the provided big_pow routine. */
    return big_pow((seq_t)prime, (seq_t)j);
}
