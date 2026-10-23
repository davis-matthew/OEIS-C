/*
 * A161464  Sum of all digits of primes from n-th prime to (2*n-1)-th prime.
 * https://oeis.org/A161464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of all digits of primes from the n‑th prime to the (2*n-1)-th prime (1‑based) */
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/digit_sum.h"

static uint64_t *primes_cache = NULL;
static size_t primes_cache_cnt = 0;

seq_t a(idx_t n)
{
    if (n == 0) { /* Undefined per OEIS offset */
        return 0;
    }

    /* Index of the last required prime (1‑based) */
    idx_t required_idx = 2 * n - 1;          /* (2*n-1)-th prime inclusive */

    /* Grow the cache if we do not have enough primes yet */
    if (primes_cache_cnt < required_idx) {
        idx_t limit = _prime_limit_for_idx(required_idx);
        uint64_t *tmp_primes = NULL;
        size_t tmp_cnt = 0;
        build_primes_upto(limit, &tmp_primes, &tmp_cnt);
        free(primes_cache);
        primes_cache = tmp_primes;
        primes_cache_cnt = tmp_cnt;
    }

    seq_t sum = 0;
    idx_t start = n - 1;            /* 0‑based index of the n‑th prime */
    idx_t end   = required_idx - 1; /* inclusive, 0‑based index of the (2*n-1)-th prime */

    for (idx_t i = start; i <= end; ++i) {
        sum += digit_sum((seq_t)primes_cache[i]);
    }
    return sum;
}
