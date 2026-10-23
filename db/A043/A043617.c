/*
 * A043617  Numbers whose base-7 representation has exactly 2 runs.
 * https://oeis.org/A043617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate integers in increasing order, keeping those whose base‑7 representation
   consists of exactly two runs of equal digits.  The runs are counted by scanning the
   digits from most significant to least and counting changes of digit.  The first
   such number is 7 (base‑7 "10") and the sequence continues in ascending order. */
#include <stdlib.h>
#include <stdint.h>

/* Checks whether a base‑7 representation of x has exactly two runs.
   The representation has no leading zeros.  */
static int has_two_runs(seq_t x)
{
    if (x == 0) return 0;

    int runs = 0;
    int last = -1;
    int digits[128];
    int len = 0;
    seq_t tmp = x;

    /* Extract base‑7 digits least‑significant first. */
    while (tmp > 0) {
        int d = (int)(tmp % 7);
        digits[len++] = d;
        tmp /= 7;
    }

    /* Count runs while scanning from most‑significant digit. */
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        if (d != last) {
            ++runs;
            last = d;
            if (runs > 2) return 0;   /* early exit */
        }
    }
    return runs == 2;
}

/* Returns the n‑th number whose base‑7 representation has exactly two runs
   (a(1) = 7).  The function memoizes results so that subsequent calls are fast. */
seq_t a(idx_t n)
{
    /* 0 is outside the range; just return 0 in that case. */
    if (n == 0) return 0;

    /* Static cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 0;   /* last integer examined */

    size_t idx = (size_t)(n - 1);    /* zero‑based index */

    if (cache_len > idx)
        return cache[idx];

    /* Grow cache to accommodate at least idx+1 terms. */
    while (cache_len <= idx) {
        /* Find next number with two runs. */
        seq_t num = last_checked + 1;
        for (;;) {
            ++last_checked;
            if (has_two_runs(last_checked)) {
                /* Ensure capacity. */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!new_cache) abort(); /* memory exhaustion */
                    cache = new_cache;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = last_checked;
                break;
            }
        }
    }
    return cache[idx];
}
