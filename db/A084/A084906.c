/*
 * A084906  Numbers with at least one place in their decimal representation to
 *          insert a division operator such that an integer results.
 * https://oeis.org/A084906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the nth term of OEIS A084906.
    The sequence is defined as the increasing positive integers m
    for which there exists a split of the decimal representation of m
    into two non‑empty parts a|b (b≠0) such that a % b == 0.
    The function generates terms on demand, storing them in a static
    cache so that calls with arbitrary n receive correct results. */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: test whether m satisfies the condition.  */
static bool is_good(seq_t m)
{
    if (m < (seq_t)10)          /* need at least two decimal digits */
        return false;

    /* Count decimal digits. */
    idx_t digits = 0;
    seq_t t = m;
    while (t > 0) {
        t /= 10;
        ++digits;
    }

    seq_t pow10 = (seq_t)10;     /* 10^1   */
    for (idx_t k = 1; k < digits; ++k) {
        seq_t suffix = m % pow10;          /* right part  */
        if (suffix > 0) {
            seq_t left = m / pow10;       /* left part   */
            if (left % suffix == 0)
                return true;
        }
        pow10 *= 10;                      /* next power of ten */
    }
    return false;
}

/*  Main sequence function  */
seq_t a(idx_t n)
{
    /* Static cache of generated terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static seq_t  last_checked = 0;  /* last integer examined */
    static idx_t  generated    = 0;  /* number of terms stored */

    if (n < 1) return (seq_t)0;  /* offset 1; undefined for n<1 */

    /* Generate terms until we have at least n of them. */
    while (generated < n) {
        ++last_checked;                       /* next integer to test */
        if (is_good(last_checked)) {
            /* Ensure capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();              /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = last_checked;
            ++generated;
        }
    }

    return cache[n - 1];
}
