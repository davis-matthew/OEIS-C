/*
 * A035139  Digits of prime factors of k do not appear in k.
 * https://oeis.org/A035139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer k such that the * digits of
 *            every prime factor of k do not appear in the decimal *
 *            representation of k. a(1)=1, a(2)=4, a(3)=6, … as listed in *
 *            OEIS A035139
 * Range:     u8 n <= 67, u16 n <= 3476, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static seq_t *cache = NULL;       /* cached sequence values */
static size_t cache_size = 0;     /* number of cached terms */
static size_t cache_cap  = 0;     /* allocated capacity of cache */
static uint64_t last_candidate = 0;   /* last integer that satisfied the condition */

/* Helper: determine whether the digits of the prime factors of n
 * are disjoint from the digits of n itself.  n is treated as an
 * unsigned 64‑bit integer; the return value is true if n fulfils
 * the requirement – i.e. if all digits of the prime factors of n
 * are absent from n.  This function may modify temporary buffers
 * but is otherwise independent of the global cache. */
static bool is_valid_factor_digits(uint64_t n)
{
    /* Special case: 1 has no prime factors and passes automatically. */
    if (n == 1)
        return true;

    /* Record the digits present in n itself. */
    bool digit_in_n[10] = {false};
    uint64_t tmp = n;
    while (tmp > 0) {
        digit_in_n[tmp % 10] = true;
        tmp /= 10;
    }

    /* Record the digits that appear in any prime factor of n. */
    bool digit_in_factor[10] = {false};

    /* Factor n into its prime constituents.  The maximum number of
     * distinct prime factors for any 64‑bit integer is 12, so a small
     * buffer of size 20 suffices. */
    uint64_t primes_buf[20];
    uint64_t exps_buf[20];
    size_t factor_cnt = 0;

    factor_u64(n, primes_buf, exps_buf, &factor_cnt);

    for (size_t i = 0; i < factor_cnt; ++i) {
        uint64_t p = primes_buf[i];
        /* Mark all decimal digits that appear in the prime factor p. */
        uint64_t q = p;
        while (q > 0) {
            digit_in_factor[q % 10] = true;
            q /= 10;
        }
    }

    /* If any digit used by a prime factor also occurs in n, n is invalid. */
    for (int d = 0; d < 10; ++d)
        if (digit_in_n[d] && digit_in_factor[d])
            return false;

    return true;
}

/* Main sequence function.  It is fully memoised: calls with any n
 * will be answered from the cache, which is dynamically extended
 * only when necessary.  The cache is built on demand. */
seq_t a(idx_t n)
{
    /* By definition the sequence is indexed starting at 1. */
    if (n == 0)
        return (seq_t)0;

    /* Expand cache if required. */
    if ((size_t)n > cache_size) {
        /* Allocate or grow the cache array. */
        size_t target = (size_t)n;
        if (target > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < target)
                new_cap = target;
            seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
            for (size_t i = 0; i < cache_size; ++i)
                new_buf[i] = cache[i];
            free(cache);
            cache = new_buf;
            cache_cap = new_cap;
        }

        /* Search for additional valid integers starting just after
         * the last candidate found previously. */
        uint64_t cand = last_candidate + 1;
        while (cache_size < target) {
            if (is_valid_factor_digits(cand)) {
                cache[cache_size++] = (seq_t)cand;
                last_candidate = cand;
            }
            ++cand;
        }
    }

    return cache[n - 1];
}
