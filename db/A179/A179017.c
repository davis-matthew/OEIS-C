/*
 * A179017  Odd numbers c such that c*(c^2 - 1)/4 is squarefree.
 * https://oeis.org/A179017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 8227, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A179017: odd numbers c such that (c^2-1)c/4 is squarefree.
 * Equivalent to generating numbers c=2a+1 where a, a+1 and 2a+1 are all squarefree.
 * The sequence is obtained lazily; it is memoised and works for calls in any order. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static bool is_squarefree(seq_t x)
{
    /* Returns true if x is squarefree.
     * Uses trial division by integers up to sqrt(x). */
    for (unsigned long long p = 2ULL; (seq_t)p * (seq_t)p <= x; ++p) {
        seq_t sq = (seq_t)p * (seq_t)p;
        if (x % sq == 0) return false;
    }
    return true;
}

static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_ptr = (seq_t*)realloc(*ptr, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(EXIT_FAILURE); /* OOM */
    *ptr = new_ptr;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0ULL; /* undefined for n<1, return 0 */

    /* static state: cache of computed terms, current a being tested */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next_a = 1; /* next a to test */

    /* Extend cache until we have at least n terms */
    while (cache_len < n) {
        seq_t a_val = (seq_t)next_a;
        seq_t a1 = a_val + 1;
        seq_t c_val = (seq_t)2 * a_val + 1;

        if (is_squarefree(a_val) && is_squarefree(a1) && is_squarefree(c_val)) {
            ensure_capacity(cache_len + 1, &cache_cap, &cache);
            cache[cache_len++] = c_val;
        }
        ++next_a;
    }

    return cache[n - 1];
}
