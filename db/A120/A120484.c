/*
 * A120484  a(n) = Sum(Sum p_i, {Sum p_i=prime(n)}, p_i is prime).
 * https://oeis.org/A120484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * (number of partitions of prime(n) into primes)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   build_primes_upto, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static uint64_t *prime_list     = NULL;
static size_t    prime_count    = 0;
static seq_t *   dp_array       = NULL;
static idx_t     dp_limit       = 0;

/* make sure prime_list contains at least n primes */
static void ensure_primes_count(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;

    idx_t limit;
    if (n < 6) {
        limit = 15;
    } else {
        double nd = (double)n;
        limit = (idx_t)(nd * (log(nd) + log(log(nd))) + 3.0);
    }

    while (1) {
        uint64_t *tmp = NULL;
        size_t    cnt = 0;
        build_primes_upto(limit, &tmp, &cnt);
        if (cnt >= (size_t)n) {
            if (prime_list)
                free(prime_list);
            prime_list  = tmp;
            prime_count = cnt;
            break;
        }
        free(tmp);
        limit *= 2;
    }
}

/* make sure prime_list contains all primes ≤ limit */
static void ensure_primes_up_to_value(idx_t limit)
{
    if (!prime_list || prime_count == 0) {
        ensure_primes_count(1);
    }

    if (prime_list[prime_count - 1] >= limit)
        return;

    while (1) {
        uint64_t *tmp = NULL;
        size_t    cnt = 0;
        build_primes_upto(limit, &tmp, &cnt);
        if (tmp && tmp[cnt - 1] >= limit) {
            if (prime_list)
                free(prime_list);
            prime_list  = tmp;
            prime_count = cnt;
            break;
        }
        free(tmp);
        limit *= 2;
    }
}

/* rebuild dp_array up to new_max, recomputing from scratch */
static void rebuild_dp(idx_t new_max)
{
    if (dp_array)
        seq_free_array(dp_array);

    dp_array = seq_alloc_new_array(new_max + 1);
    dp_array[0] = 1;
    for (idx_t i = 1; i <= new_max; ++i)
        dp_array[i] = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p > new_max)
            break;
        for (idx_t s = p; s <= new_max; ++s)
            dp_array[s] += dp_array[s - p];
    }
    dp_limit = new_max;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    ensure_primes_count(n);

    idx_t prime_val = prime_list[n - 1];

    if (prime_val > dp_limit) {
        ensure_primes_up_to_value(prime_val);
        rebuild_dp(prime_val);
    }

    seq_t p = (seq_t)prime_val;
    return p * dp_array[prime_val];
}
