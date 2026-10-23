/*
 * A048319  Numbers whose base-8 expansions, read from left to right, have run
 *          lengths that strictly decrease.
 * https://oeis.org/A048319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A048319: Numbers whose base‑8 expansions, read from left to right,
   have run lengths that strictly decrease.
   Implementation: generate candidates sequentially, check each by
   inspecting its base‑8 digits, and cache all results for subsequent calls. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache = NULL;        /* cached sequence values            */
static size_t  cache_size = 0;     /* number of cached elements         */
static size_t  cache_cap  = 0;     /* allocated capacity                */

static bool is_good(seq_t x)
{
    if (x == 0) return true;              /* zero is valid by convention   */
    /* Extract base‑8 digits into a temporary array (max. 128 digits is ample). */
    unsigned char digits[128];
    idx_t len = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % 8);
        tmp /= 8;
    }
    if (len == 0) return true;            /* safety, shouldn't happen       */

    /* Scan runs from most significant digit to least. */
    idx_t i;
    int   curr_digit   = digits[len - 1];
    int   run_len      = 1;
    int   prev_run_len = -1;               /* -1 indicates no previous run */

    for (i = 1; i < len; ++i) {
        int next_digit = digits[len - 1 - i];
        if (next_digit == curr_digit) {
            ++run_len;
        } else {
            if (prev_run_len != -1 && run_len >= prev_run_len)
                return false;
            prev_run_len = run_len;
            curr_digit   = next_digit;
            run_len      = 1;
        }
    }
    /* Last run must also be strictly shorter. */
    if (prev_run_len != -1 && run_len >= prev_run_len)
        return false;
    return true;
}

static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                    /* fatal if memory allocation fails*/
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Lazy–initialisation of the cache with the first term (0). */
    if (cache == NULL) {
        ensure_cache_cap(1);
        cache[0] = 0;
        cache_size = 1;
    }

    /* Extend cache until we have computed a( n ). */
    while (cache_size <= n) {
        /* Find next valid number after the last cached value. */
        seq_t candidate = cache[cache_size - 1] + 1;
        while (!is_good(candidate))
            ++candidate;

        ensure_cache_cap(cache_size + 1);
        cache[cache_size++] = candidate;
    }

    return cache[n];
}
