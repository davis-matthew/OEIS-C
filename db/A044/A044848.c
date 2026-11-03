/*
 * A044848  Positive integers having no fewer base-7 runs of even length than
 *          odd.
 * https://oeis.org/A044848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: compute a(n) of A044848 (positive integers with no fewer
 * base‑7 runs of even length than odd) by brute‑force enumeration.
 * The nth term is found by scanning all positive integers and caching
 * the valid ones.  This yields O(n²) value for a(n), but for n ≤ 10 000
 * the runtime is well below two seconds. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: test whether a number has at least as many even‑length
 * base‑7 runs as odd‑length runs. */
static bool even_runs_at_least_odd(seq_t x)
{
    if (x == 0) return false;                     /* 0 is not positive   */

    uint8_t digits[32];                           /* enough for 64‑bit      */
    int dlen = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        digits[dlen++] = (uint8_t)(tmp % 7);
        tmp /= 7;
    }

    size_t evens = 0, odds = 0;
    int idx = dlen - 1;                           /* most significant digit */
    int prev = digits[idx];
    int runlen = 1;

    for (idx = dlen - 2; idx >= 0; --idx) {
        if (digits[idx] == prev)
            ++runlen;
        else {
            if ((runlen & 1) == 0) ++evens;
            else ++odds;
            prev = digits[idx];
            runlen = 1;
        }
    }
    /* last run */
    if ((runlen & 1) == 0) ++evens;
    else ++odds;

    return evens >= odds;
}

/* Cache for generated terms */
static seq_t *cache = NULL;        /* dynamically allocated array */
static size_t cache_len = 0;       /* number of terms stored */
static size_t cache_cap = 0;       /* allocated capacity            */
static seq_t next_to_check = 1;    /* next integer to test         */

/* Ensure that at least N terms are cached. */
static void ensure_terms(idx_t N)
{
    if ((idx_t)cache_len >= N) return;

    /* Grow cache capacity if needed */
    if (cache_cap < (size_t)N) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < (size_t)N) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();                /* memory exhausted     */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Generate successive integers until N terms are found */
    seq_t val;
    for (val = next_to_check; cache_len < (size_t)N; ++val) {
        if (even_runs_at_least_odd(val)) {
            cache[cache_len++] = val;
        }
    }
    next_to_check = val;
}

/* Public function matching OEIS entry */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n = 0   */
    ensure_terms(n);
    return cache[n - 1];
}
