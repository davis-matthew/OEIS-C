/*
 * A131698  Cumulative concatenation of A000032 Lucas numbers (beginning at
 *          2).
 * https://oeis.org/A131698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1)=2; a(n+1)=concat(a(n),L(n+1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* L(k) are Lucas numbers: L(0)=2, L(1)=1, L(k)=L(k-1)+L(k-2) */

#include <stdlib.h>
#include <stdint.h>             /* for uint64_t if needed */
#include <stdbool.h>            /* for bool */
#include "../utils/digit_len.h" /* int digit_len(seq_t x) */
#include "../utils/pow10.h"     /* seq_t pow10(idx_t e) */

/* seq_t and idx_t are supplied by the build system                         */
/* seq_t is either a built‑in unsigned type or an arbitrary‑precision class */

static seq_t *a_cache      = NULL; /* a_cache[i] = a(i)  (1‑based) */
static seq_t *lucas_cache  = NULL; /* lucas_cache[k] = L(k)  (0‑based) */
static size_t a_cap        = 0;
static size_t lucas_cap    = 0;
static idx_t  a_max        = 0;     /* largest n computed in a_cache  */
static idx_t  lucas_max    = 0;     /* largest k computed in lucas_cache */

/* Ensure a_cache can hold indices up to 'need' (inclusive).               */
static void ensure_a_cap(idx_t need)
{
    if (a_cap > 0 && a_cap >= (size_t)(need + 2))
        return;
    size_t newcap = (size_t)(need + 2) * 2;
    a_cache = (seq_t *)realloc(a_cache, newcap * sizeof(seq_t));
    a_cap   = newcap;
}

/* Ensure lucas_cache can hold indices up to 'need' (inclusive).           */
static void ensure_lucas_cap(idx_t need)
{
    if (lucas_cap > 0 && lucas_cap >= (size_t)(need + 2))
        return;
    size_t newcap = (size_t)(need + 2) * 2;
    lucas_cache = (seq_t *)realloc(lucas_cache, newcap * sizeof(seq_t));
    lucas_cap   = newcap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* Undefined for n < 1; caller must provide n ≥ 1 */

    /* Initialise caches on first call.                               */
    if (a_cache == NULL) {
        ensure_a_cap(1);
        a_cache[1] = (seq_t)2;  /* a(1) = 2 */
        a_max = 1;

        ensure_lucas_cap(1);
        lucas_cache[0] = (seq_t)2;  /* L(0) */
        lucas_cache[1] = (seq_t)1;  /* L(1) */
        lucas_max = 1;
    }

    /* Compute missing Lucas numbers up to L(n-1).                     */
    if (lucas_max < n - 1) {
        ensure_lucas_cap(n - 1);
        for (idx_t k = lucas_max + 1; k <= n - 1; ++k)
            lucas_cache[k] = lucas_cache[k - 1] + lucas_cache[k - 2];
        lucas_max = n - 1;
    }

    /* Compute missing a(k) values up to a(n).                         */
    if (a_max < n) {
        ensure_a_cap(n);
        for (idx_t i = a_max + 1; i <= n; ++i) {
            seq_t   prev_a = a_cache[i - 1];
            seq_t   l_val  = lucas_cache[i - 1];
            int     d_len  = digit_len(l_val);
            seq_t   mult   = prev_a * pow10((idx_t)d_len);
            a_cache[i]    = mult + l_val;
        }
        a_max = n;
    }

    return a_cache[n];
}
