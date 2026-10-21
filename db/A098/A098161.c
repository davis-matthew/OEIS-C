/*
 * A098161  Slowest increasing sequence where the digits fit the infinite
 *          repeating pattern even/even/odd/odd/even/even/odd/odd/...
 * https://oeis.org/A098161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A098161: Slowest increasing sequence with digits following the
   repeating pattern even/even/odd/odd.  The first term is 0 (index 0).
   For n > 0 each term is the smallest integer greater than the
   previous term such that, when all numbers a(0)…a(n) are concatenated
   in base 10, every digit at position p (starting at 1) satisfies:
     * p % 4 == 1 or 2  → even digit
     * otherwise        → odd digit.
   The function caches previously computed terms, so any order of calls
   is supported.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- helper functions ------------------------------------- */

static seq_t pow10(idx_t e)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < e; ++i) r *= (seq_t)10;
    return r;
}

static int digit_len(seq_t x)
{
    int len = 0;
    do {
        ++len;
        x /= (seq_t)10;
    } while (x != (seq_t)0);
    return len;
}

/* ---------- main sequence function ------------------------------- */

seq_t a(idx_t n)
{
    /* static cache and state */
    static seq_t *cache = NULL;
    static size_t   cap    = 0;
    static size_t   sz     = 0;
    static idx_t    total_digits = 0;   /* digits used by stored terms */

    /* initialise on first call */
    if (sz == 0) {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();                 /* unreachable in BOJ tests   */
        cache[0] = (seq_t)0;                 /* a(0) = 0 */
        sz = 1;
        total_digits = 1;                    /* one digit of '0'          */
    }

    /* return cached value if already known */
    if (n < sz) return cache[(size_t)n];

    /* generate terms up to index n */
    for (idx_t idx = sz; idx <= n; ++idx) {
        seq_t cand = cache[sz - 1] + (seq_t)1;  /* start searching            */

        for (;;) {
            int   len = digit_len(cand);
            idx_t start_pos = total_digits + (idx_t)1;   /* next free position */
            seq_t div = pow10((idx_t)(len - 1));
            int ok = 1;

            /* test each digit against the pattern */
            for (int j = 0; j < len; ++j) {
                seq_t d  = (cand / div) % (seq_t)10;
                idx_t pos = start_pos + (idx_t)j;
                int need_even = (pos % (idx_t)4 == (idx_t)1) ||
                                (pos % (idx_t)4 == (idx_t)2);
                if ((need_even && ((int)(d & (seq_t)1) != 0)) ||
                    (!need_even && ((int)(d & (seq_t)1) != 1))) {
                    ok = 0;
                    break;
                }
                div /= (seq_t)10;
            }

            if (ok) break;           /* candidate matches the pattern   */
            ++cand;                  /* otherwise try next integer       */
        }

        /* ensure capacity for new value */
        if (sz == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap   = new_cap;
        }

        cache[sz++] = cand;
        total_digits += (idx_t)digit_len(cand);
    }

    return cache[(size_t)n];
}
