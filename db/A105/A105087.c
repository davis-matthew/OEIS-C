/*
 * A105087  Absolute difference between the sums of the left and right
 *          diagonals of ordered 2 X 2 prime squares.
 * https://oeis.org/A105087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = |(p_{4n-3} + p_{4n}) - (p_{4n-2} + p_{4n-1})|
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p_k is the k-th prime.                               */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/estimate_prime_upper_bound.h"

static uint64_t *prime_pool = NULL;
static size_t prime_count = 0;

/* Ensure that prime_pool contains at least "need" primes. */
static void ensure_primes(size_t need)
{
    if (prime_count >= need)
        return;

    /* Estimate an upper bound for the "need"-th prime. */
    uint64_t ub = estimate_prime_upper_bound(need);
    if (ub == 0) /* safety fall back */
        ub = need * 20 + 100;
    size_t limit = (size_t)ub;

    /* Allocate array to mark composites. */
    uint8_t *is_comp = (uint8_t*)calloc(limit + 1, 1);
    if (!is_comp)
        exit(EXIT_FAILURE);

    /* Allocate new primes array. */
    uint64_t *new_primes = (uint64_t*)malloc(need * sizeof(uint64_t));
    if (!new_primes) {
        free(is_comp);
        exit(EXIT_FAILURE);
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (cnt < need)
                new_primes[cnt++] = (uint64_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }

    free(is_comp);

    /* Replace old pool. */
    free(prime_pool);
    prime_pool = new_primes;
    prime_count = cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* behavior for out-of-range is undefined */

    size_t need_primes = 4 * (size_t)n;
    ensure_primes(need_primes);

    /* 1‑based prime indices: p_{4n-3}, p_{4n-2}, p_{4n-1}, p_{4n} */
    size_t a_idx = 4 * n - 4;   /* 0‑based index for p_{4n-3} */
    size_t b_idx = 4 * n - 3;   /* 0‑based index for p_{4n-2} */
    size_t c_idx = 4 * n - 2;   /* 0‑based index for p_{4n-1} */
    size_t d_idx = 4 * n - 1;   /* 0‑based index for p_{4n  } */

    uint64_t left  = prime_pool[a_idx] + prime_pool[d_idx];
    uint64_t right = prime_pool[b_idx] + prime_pool[c_idx];
    uint64_t diff  = (left > right) ? (left - right) : (right - left);

    return (seq_t)diff;   /* cast to seq_t is safe; values fit in 64‑bit */
}
