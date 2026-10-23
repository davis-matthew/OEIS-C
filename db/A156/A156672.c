/*
 * A156672  a(1)=2, a(n+1) is the smallest prime >= a(n) + sum of digits of
 *          a(n).
 * https://oeis.org/A156672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(1)=2 and for n≥1, a(n+1) = the smallest prime ≥
 *            a(n)+sum_of_digits(a(n))
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq, digit_sum, free_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"
#include "../utils/digit_sum.h"
#include "../utils/is_prime_big.h"

/* Compute the next term given the previous term */
static seq_t next_term(seq_t prev) {
    seq_t target = prev + digit_sum(prev);   /* a(n) + sum of digits of a(n) */
    seq_t p = target;
    while (!is_prime_big(p))                 /* find smallest prime ≥ target */
        ++p;
    return p;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;                    /* offset starts at 1, undefined for 0 */

    /* Static memoization */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;
    static size_t cache_cap = 0;

    /* Expand cache if necessary */
    if (cache == NULL || cache_cap < (size_t)(n + 1)) {
        seq_t *new_cache = alloc_seq((size_t)(n + 1));
        if (cache != NULL) {
            /* copy existing values */
            for (idx_t i = 1; i <= max_n; ++i)
                new_cache[i] = cache[i];
            free_seq(cache);
        }
        cache = new_cache;
        cache_cap = (size_t)(n + 1);
    }

    /* Generate missing terms */
    if (n > max_n) {
        if (max_n < 1) {
            cache[1] = 2;                   /* a(1)=2 */
            max_n = 1;
        }
        for (idx_t i = max_n + 1; i <= n; ++i)
            cache[i] = next_term(cache[i - 1]);
        max_n = n;
    }

    return cache[n];
}
