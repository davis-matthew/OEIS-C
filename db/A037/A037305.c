/*
 * A037305  Numbers whose base-2 and base-7 expansions have the same digit
 *          sum.
 * https://oeis.org/A037305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose binary and base‑7 digit sums are equal (OEIS A037305). */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#ifndef __cplusplus
/* In plain C, use malloc/realloc for dynamic arrays. */
static seq_t *cache = NULL;          /* array of found terms */
static size_t cache_cap = 0;         /* capacity of the array     */
static size_t cache_size = 0;        /* number of terms stored    */
static seq_t next_candidate = 1;     /* next integer to test      */
#else
/* In C++, use a std::vector for ease of use with arbitrary‑precision types. */
#include <vector>
static std::vector<seq_t> cache_vec;
static seq_t next_candidate = 1;
#endif

/* Allocate or enlarge the cache for C. */
#ifndef __cplusplus
static void cache_ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}
#endif

/* Sum of binary (base‑2) digits of x. */
static inline seq_t digit_sum_base2(seq_t x)
{
    seq_t sum = 0;
    while (x != 0) {
        sum += (x & 1);
        x >>= 1;
    }
    return sum;
}

/* Sum of base‑7 digits of x. */
static inline seq_t digit_sum_base7(seq_t x)
{
    seq_t sum = 0;
    while (x != 0) {
        sum += (x % 7);
        x /= 7;
    }
    return sum;
}

/* Return the n‑th term of A037305 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined; return 0 to avoid UB */
        return 0;

    size_t target = (size_t)n;

#ifndef __cplusplus
    /* Grow cache array if necessary. */
    cache_ensure_capacity(target);

    /* Generate terms until we have `target` of them. */
    while (cache_size < target) {
        seq_t x = next_candidate;
        if (digit_sum_base2(x) == digit_sum_base7(x)) {
            cache[cache_size++] = x;
        }
        next_candidate++;
    }

    return cache[target - 1];
#else
    /* In C++, rely on std::vector. */
    while (cache_vec.size() < target) {
        seq_t x = next_candidate;
        if (digit_sum_base2(x) == digit_sum_base7(x)) {
            cache_vec.push_back(x);
        }
        next_candidate++;
    }
    return cache_vec[target - 1];
#endif
}
