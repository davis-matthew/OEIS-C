/*
 * A032978  Numbers with the property that all pairs of consecutive base-7
 *          digits differ by 0 or 1.
 * https://oeis.org/A032978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032978: numbers whose base-7 representation has every pair of
 * adjacent digits differing by 0 or 1 (search algorithm). */
#include <stddef.h>
#include <stdlib.h>

/* Helper: store a value in the static cache, expanding it if necessary. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void add_to_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Recursive generation of all numbers of a given length that satisfy the
 * digit-difference rule.  `val` is the value built so far, `last` is the
 * last digit used, and `rem` is how many more digits to add. */
static void generate_rec(seq_t val, int last, int rem)
{
    if (rem == 0) {
        add_to_cache(val);
        return;
    }
    int start = (last > 0) ? last - 1 : 0;
    int end   = (last < 6) ? last + 1 : 6;
    for (int d = start; d <= end; ++d) {
        seq_t new_val = val * (seq_t)7 + (seq_t)d;
        generate_rec(new_val, d, rem - 1);
    }
}

/* Ensure that the cache contains at least `need` terms.  The cache grows
 * by length, generating all valid numbers with that many base‑7 digits. */
static size_t current_len = 0;

static void generate_upto(idx_t need)
{
    if (cache_len >= (size_t)need) return;

    while (cache_len < (size_t)need) {
        ++current_len;
        if (current_len == 1) {
            /* single–digit numbers: 1,2,3,4,5,6 */
            for (int d = 1; d <= 6; ++d) add_to_cache(d);
        } else {
            for (int first = 1; first <= 6; ++first)
                generate_rec(first, first, current_len - 1);
        }
    }
}

/* Required exported function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0; return 0 as a safe value */
    if (cache_len < (size_t)n) generate_upto(n);
    return cache[n - 1];
}
