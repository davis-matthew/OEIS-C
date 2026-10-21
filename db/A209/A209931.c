/*
 * A209931  Numbers k such that smallest digit of all divisors of k is 1.
 * https://oeis.org/A209931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 208, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A209931: numbers k such that the smallest decimal digit among all
   divisors of k is 1.  We generate the sequence by testing each positive
   integer for the presence of a zero‑digit divisor. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Check whether a number contains the digit 0. */
static bool has_zero_digit(seq_t x)
{
    if (x == 0)          /* divisor 0 is never reached, but guard */
        return true;
    while (x != 0) {
        if ((x % 10) == 0)
            return true;
        x = x / 10;
    }
    return false;
}

/* Return the nth term (1‑based) of A209931. */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL; /* array of found terms   */
    static size_t cache_cap = 0;  /* capacity of cache      */
    static size_t cache_len = 0;  /* number of terms cached */
    static seq_t cur = 1;         /* next candidate to test */

    /* Ensure 1‑based indexing.  Behaviour for n==0 is undefined. */
    if (n == 0)
        return (seq_t)0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        bool ok = true;

        /* Quick rejection: k itself must not contain zero. */
        if (!has_zero_digit(cur)) {
            /* Check all divisor pairs (d, e = cur/d). */
            for (seq_t d = (seq_t)1; d <= cur / d; ++d) {
                if (cur % d == 0) {
                    seq_t e = cur / d;
                    if (has_zero_digit(d))
                        { ok = false; break; }
                    if (e != d && has_zero_digit(e))
                        { ok = false; break; }
                }
            }
        } else {
            ok = false;
        }

        if (ok) {
            /* Grow cache if necessary. */
            if (cache_len == cache_cap) {
                size_t newcap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_len++] = cur;
        }
        ++cur;   /* move to next candidate integer */
    }

    return cache[n - 1];
}
