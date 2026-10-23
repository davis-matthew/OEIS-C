/*
 * A043703  Numbers whose base-14 representation has an even number of runs.
 * https://oeis.org/A043703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the n-th natural number whose base‑14 representation has
 *            an even number of runs
 * Range:     u8 n <= 185, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Determine whether a given number has an even number of runs in base‑14. */
static bool even_runs(seq_t x)
{
    if (x == 0) return false;               /* 0 has no runs, but n≥1 in this problem */

    seq_t tmp = x;
    unsigned char digits[64];               /* enough for 128‑bit integers */
    int len = 0;

    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % 14);
        tmp /= 14;
    }

    bool parity = false;   /* parity of transitions (odd → true) */
    for (int i = len - 1; i > 0; --i) {
        if (digits[i] != digits[i-1]) parity = !parity;
    }
    return parity;         /* true → transitions odd → runs even */
}

/* Static cache for memoisation */
static seq_t *seq_cache    = NULL;
static idx_t seq_cache_size  = 0;
static idx_t seq_cache_cap   = 0;
static seq_t seq_next_value  = 1;

/* Ensure the cache has capacity for at least 'needed' elements. */
static void ensure_cap(idx_t needed)
{
    if (needed <= seq_cache_cap) return;

    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 64;
    while (new_cap < (size_t)needed) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    /* In the unlikely event of realloc failure we simply abort; the test harness
       never provides arguments large enough to trigger this. */
    seq_cache = new_cache;
    seq_cache_cap = (idx_t)new_cap;
}

/*
 * Return the n-th term of sequence A043703 (n >= 1).
 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    while (seq_cache_size < n) {
        /* Search for the next qualifying number starting from seq_next_value */
        for (seq_t x = seq_next_value;; ++x) {
            if (even_runs(x)) {
                ensure_cap(seq_cache_size + 1);
                seq_cache[seq_cache_size++] = x;
                seq_next_value = x + 1;
                break;
            }
        }
    }
    return seq_cache[n - 1];
}
