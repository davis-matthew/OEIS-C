/*
 * A202444  Number of (n+2) X 7 binary arrays avoiding patterns 001 and 110 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 11, u32 n <= 139, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A202444 – Number of (n+2) × 7 binary arrays avoiding
 *  patterns 001 and 110 in rows, columns and NW‑SE diagonals.
 *
 *  Recurrence (valid for n > 10):
 *      a(n) = 3*a(n-1) - 8*a(n-3) + 6*a(n-4) + 6*a(n-5)
 *            - 8*a(n-6) + 3*a(n-8) - a(n-9)
 *
 *  Initial values:
 *      a(1)=484, a(2)=836, a(3)=1722, a(4)=3122, a(5)=5512,
 *      a(6)=8996, a(7)=14338, a(8)=21674, a(9)=32120,
 *      a(10)=45844.
 *
 *  The function uses a static memoised cache so that repeated calls
 *  are fast and calls may appear in any order.  All arithmetic is
 *  performed using the type seq_t provided by the build environment,
 *  which may be an exact integer type or an arbitrary‑precision
 *  integer class; the operations +, -, * are defined accordingly.
 */

#include <stdlib.h>

/* static cache of previously computed values (a(1) stored at index 0) */
static seq_t *cache = NULL;   /* cache[i] holds a(i+1) */
static idx_t cache_size = 0;  /* number of entries currently stored */

/* initialise or extend the cache up to at least n entries (1‑based) */
static void ensure_cache(idx_t n)
{
    if (cache_size >= n)
        return;

    /* grow the cache */
    seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (tmp == NULL) {
        exit(EXIT_FAILURE);   /* allocation failed */
    }
    cache = tmp;

    /* base values for a(1) … a(10) */
    static const seq_t base[10] = {
        484ULL, 836ULL, 1722ULL, 3122ULL, 5512ULL,
        8996ULL, 14338ULL, 21674ULL, 32120ULL, 45844ULL
    };

    /* compute values from the current size up to n-1 (0‑based index) */
    idx_t i;
    for (i = cache_size; i < n; ++i) {
        if (i < 10) {
            cache[i] = base[i];
        } else {
            /* recurrence: a(i+1) = 3*a(i) - 8*a(i-2) + 6*a(i-3) + 6*a(i-4)
             *                 - 8*a(i-5) + 3*a(i-7) - a(i-8)          */
            seq_t term = 3 * cache[i - 1]
                       - 8 * cache[i - 3]
                       + 6 * cache[i - 4]
                       + 6 * cache[i - 5]
                       - 8 * cache[i - 6]
                       + 3 * cache[i - 8]
                       - cache[i - 9];
            cache[i] = term;
        }
    }

    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined outside the sequence */
    ensure_cache(n);
    return cache[n - 1];
}
