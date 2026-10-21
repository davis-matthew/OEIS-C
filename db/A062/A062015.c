/*
 * A062015  Numbers with all even digits which can be factored into two
 *          numbers with all even digits.
 * https://oeis.org/A062015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer with all even digits that has a non‑trivial *
 *            factorisation into two integers each having all even digits
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   all_even_digits, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/all_even_digits.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;
static idx_t next_candidate = 1; /* next integer to examine */

static bool has_even_digit_factor(seq_t x)
{
    for (idx_t d_int = 2; (seq_t)d_int * (seq_t)d_int <= x; ++d_int)
    {
        if (x % (seq_t)d_int == 0)
        {
            seq_t other = x / (seq_t)d_int;
            if (all_even_digits((seq_t)d_int) && all_even_digits(other))
                return true;
        }
    }
    return false;
}

static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

static void build_sequence(idx_t target_n)
{
    while (cache_len < target_n)
    {
        seq_t val = (seq_t)next_candidate;
        if (all_even_digits(val) && has_even_digit_factor(val))
        {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = val;
        }
        ++next_candidate;
    }
}

extern seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    if (cache == NULL)
    {
        cache_cap = 64;
        cache = seq_alloc_new_array(cache_cap);
        cache[0] = 0;   /* the first term is 0 */
        cache_len = 1;
    }

    if (cache_len < n)
        build_sequence(n);

    return cache[n - 1];
}
