/*
 * A183108  Numbers m such that sum of divisors of m and sum of palindromic
 *          divisors of m are both palindromic.
 * https://oeis.org/A183108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth positive integer m such that sigma(m) (the sum of all
 *            divisors of m) and A088000(m) (the sum of palindromic divisors
 *            of m) are both palindromic
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   factor_u64, is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_palindrome.h"
#include "../utils/factor_u64.h"

/* Helper: recursive generation of all divisors, accumulating
   the sum of all divisors (sum_all) and the sum of palindromic
   divisors (sum_pal).  The current value is passed as cur. */
static inline void
generate_divisors(const uint64_t *pr,
                  const uint64_t *exp,
                  size_t idx,
                  size_t cnt,
                  seq_t cur,
                  seq_t *sum_all,
                  seq_t *sum_pal)
{
    if (idx == cnt) {
        *sum_all += cur;
        if (is_palindrome(cur, 10))
            *sum_pal += cur;
        return;
    }

    seq_t next = cur;
    uint64_t p = pr[idx];
    for (uint64_t e = 0; e <= exp[idx]; ++e) {
        generate_divisors(pr, exp, idx + 1, cnt, next, sum_all, sum_pal);
        next *= p;
    }
}

/* Test whether m satisfies the property: both sigma(m) and
   the sum of palindromic divisors are palindromic. */
static inline bool
good_number(uint64_t m)
{
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(m, pr, ex, &cnt);

    seq_t sum_all = 0;
    seq_t sum_pal = 0;
    generate_divisors(pr, ex, 0, cnt, 1, &sum_all, &sum_pal);

    return is_palindrome(sum_all, 10) && is_palindrome(sum_pal, 10);
}

/* Main function: returns the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; undefined for 0, but return 0 */
        return 0ull;

    /* Persistently store computed terms across calls. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Return from cache if already computed. */
    if ((idx_t)cache_len >= n)
        return cache[n - 1];

    /* Determine the starting candidate for search. */
    uint64_t candidate = (cache_len == 0) ? 1 : (uint64_t)cache[cache_len - 1] + 1;

    /* Ensure capacity for new terms. */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Keep generating until the nth term is found. */
    while ((idx_t)cache_len < n) {
        if (good_number(candidate)) {
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache,
                                         cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)candidate;
        }
        candidate++;
    }

    return cache[n - 1];
}
