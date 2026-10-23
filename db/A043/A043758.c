/*
 * A043758  Numbers n such that number of runs in the base 2 representation of
 *          n is congruent to 4 mod 9.
 * https://oeis.org/A043758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer such that number_of_runs_in_base2(n) ≡ 4
 *            (mod 9)
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Count the runs in the binary representation of m. */
static inline unsigned run_count(idx_t m)
{
    unsigned runs = 0;
    int prev = -1;
    while (m != 0U) {
        int bit = m & 1U;
        if (bit != prev) {
            ++runs;
            prev = bit;
        }
        m >>= 1U;
    }
    return runs;
}

#ifdef __cplusplus
/* C++: use a std::vector to hold the sequence values. */
static std::vector<seq_t> cache;
static idx_t last_checked = 1;

static void ensure_cache(size_t needed)
{
    if (cache.size() < needed)
        cache.reserve(needed);
}
#else
/* C: use a raw dynamically allocated array. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t last_checked = 1;

static void ensure_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *ptr = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
    if (!ptr)
        abort();          /* allocation failed */
    cache      = ptr;
    cache_cap  = new_cap;
}
#endif

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; defensive return */
        return 0;

    ensure_cache((size_t)n);

#ifdef __cplusplus
    size_t len = cache.size();
#else
    size_t len = cache_len;
#endif

    while (len < (size_t)n) {
        idx_t m = last_checked;
        while ((run_count(m) % 9) != 4)
            ++m;

#ifdef __cplusplus
        cache.push_back((seq_t)m);
#else
        cache[cache_len++] = (seq_t)m;
#endif
        ++len;
        last_checked = m + 1;
    }

#ifdef __cplusplus
    return cache[n - 1];
#else
    return cache[n - 1];
#endif
}
