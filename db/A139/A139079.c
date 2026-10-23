/*
 * A139079  Smallest number not yet in the sequence that contains the last
 *          digit of the previous term, with a(1) = 1.
 * https://oeis.org/A139079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 213, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A139079:
 *  a(1) = 1
 *  a(n) = smallest unused positive integer that contains the last decimal digit
 *          of a(n‑1)  (for n > 1)
 *  This is implemented with a memoised table and a bitmap of already used
 *  integers.  The lookup is O(1) per term after the initial allocation.
 */

#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache = NULL;        /* memoised terms  */
static size_t cache_cap = 0;       /* allocated capacity of cache */
static size_t cache_len = 0;       /* number of terms currently stored */

static bool *used = NULL;          /* bitmap of used integers */
static size_t used_cap = 0;        /* allocated capacity of used bitmap */

static uint64_t last_candidate[10] = {1,1,1,1,1,1,1,1,1,1}; /* next candidate for each last digit */

/* Ensure that used[0..need] is allocated. */
static void grow_used(uint64_t need)
{
    if (need < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap << 1 : 4;
    while (new_cap <= need)
        new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp) exit(1);
    for (size_t i = used_cap; i < new_cap; ++i)
        tmp[i] = false;
    used = tmp;
    used_cap = new_cap;
}

/* Ensure that cache[0..new_cap-1] is allocated. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t cap = cache_cap ? cache_cap << 1 : 4;
    while (cap < new_cap)
        cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = cap;
}

/* Test whether x (decimal) contains the single decimal digit d. */
static bool contains_digit(seq_t x, unsigned d)
{
    if (x == 0)
        return d == 0;
    while (x > 0) {
        if ((unsigned)(x % 10u) == d)
            return true;
        x /= 10u;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n<1 */

    /* Grow the cache until it contains term n. */
    while (cache_len < (size_t)n) {
        seq_t prev = (cache_len == 0) ? (seq_t)1 : cache[cache_len - 1];
        unsigned last_digit = (unsigned)(prev % 10u);

        /* Special case: the first term a(1). */
        if (cache_len == 0) {
            grow_cache(1);
            grow_used(1);
            cache[0] = (seq_t)1;
            used[1] = true;
            last_candidate[1] = 2;  /* next candidate after 1 */
            ++cache_len;
            continue;
        }

        /* Find the smallest unused integer containing last_digit. */
        uint64_t cand = last_candidate[last_digit];
        while (true) {
            grow_used(cand);
            if (!used[cand] && contains_digit((seq_t)cand, last_digit))
                break;
            ++cand;
        }

        seq_t val = (seq_t)cand;
        grow_cache(cache_len + 1);
        cache[cache_len] = val;
        grow_used(cand);
        used[cand] = true;
        last_candidate[last_digit] = cand + 1;
        ++cache_len;
    }

    return cache[(size_t)n - 1];
}
