/*
 * A044850  Positive integers having no fewer base-9 runs of even length than
 *          odd.
 * https://oeis.org/A044850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044850: a(n) is the nth positive integer for which, in its base‑9 representation, the number of runs of even length is at least the number of runs of odd length.
   The implementation uses a simple brute‑force search. */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_checked = 0;

static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 2;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

static bool passes(seq_t x)
{
    int odd = 0, even = 0;
    int prev_digit = -1, run_len = 0;
    while (x > 0)
    {
        int digit = (int)(x % 9);
        x /= 9;
        if (digit == prev_digit)
        {
            run_len++;
        }
        else
        {
            if (prev_digit != -1)
            {
                (run_len & 1) ? odd++ : even++;
            }
            prev_digit = digit;
            run_len = 1;
        }
    }
    if (run_len > 0)
    {
        (run_len & 1) ? odd++ : even++;
    }
    return even >= odd;
}

static void ensure_n_terms(idx_t n)
{
    while (cache_len < (size_t)n)
    {
        last_checked++;
        if (passes(last_checked))
        {
            grow_cache(cache_len + 1);
            cache[cache_len++] = last_checked;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_n_terms(n);
    return cache[(size_t)(n - 1)];
}
