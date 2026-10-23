/*
 * A045056  Numbers whose base-4 representation contains exactly three 0's and
 *          four 2's.
 * https://oeis.org/A045056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose base-4 representation contains exactly three 0's
 *            and four 2's
 * Range:     u8 none, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Current length of base‑4 numbers being generated */
static idx_t cur_len = 7;

/* Append a value to the cache */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Recursive generation of numbers of length L with the required digit counts */
static void rec(idx_t pos, idx_t L, idx_t zeros_left, idx_t twos_left, seq_t val)
{
    if (pos == L) {
        if (zeros_left == 0 && twos_left == 0)
            append_cache(val);
        return;
    }

    idx_t rem = L - pos - 1;            /* positions left after this one */

    for (int d = 0; d < 4; ++d) {
        if (pos == 0 && d == 0)
            continue;                  /* leading zero not allowed */

        if (d == 0 && zeros_left == 0)
            continue;
        if (d == 2 && twos_left == 0)
            continue;

        /* Check if we can still satisfy the remaining counts */
        if ((zeros_left - (d == 0)) > rem || (twos_left - (d == 2)) > rem)
            continue;

        seq_t next_val = val * 4 + d;
        rec(pos + 1, L, zeros_left - (d == 0), twos_left - (d == 2), next_val);
    }
}

/* Generate all valid numbers of a given length and store them in cache */
static void generate_length(idx_t L)
{
    rec(0, L, 3, 4, 0);
}

/* Ensure that the cache contains at least n terms */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        generate_length(cur_len);
        ++cur_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for offset 0 */

    ensure_cache(n);
    return cache[n - 1];
}
