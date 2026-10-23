/*
 * A174505  Continued fraction expansion for exp( Sum_{n>=1} 1/(n*Lucas(n)) ),
 *          where Lucas(n) = A000032(n) = ((1+sqrt(5))/2)^n +
 *          ((1-sqrt(5))/2)^n.
 * https://oeis.org/A174505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, nonn, easy
 * Formula:   a(0)=3; for n≥1: a(3k‑2)=1, a(3k‑1)=Lucas(2k+1)-1,
 *            a(3k)=Lucas(2k+2)-1 where Lucas(0)=2, Lucas(1)=1 and
 *            Lucas(m)=Lucas(m‑1)+Lucas(m‑2)
 * Range:     u8 n <= 14, u16 n <= 32, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *lucas_cache = NULL;        /* stored Lucas numbers */
static size_t lucas_cache_cap = 0;       /* allocated capacity */
static idx_t  lucas_cache_max = 0;       /* highest index computed */

/* Ensure the cache contains Lucas numbers up to index n. */
static void ensure_lucas(idx_t n)
{
    if (lucas_cache == NULL) {
        /* initialise cache with L(0)=2, L(1)=1 */
        lucas_cache_cap = 2;
        lucas_cache = (seq_t *)malloc(lucas_cache_cap * sizeof(seq_t));
        lucas_cache[0] = (seq_t)2;
        lucas_cache[1] = (seq_t)1;
        lucas_cache_max = 1;
    }
    while (lucas_cache_max < n) {
        if (lucas_cache_cap <= lucas_cache_max + 1) {
            size_t new_cap = lucas_cache_cap * 2;
            if (new_cap == 0) new_cap = 2;
            lucas_cache = (seq_t *)realloc(lucas_cache, new_cap * sizeof(seq_t));
            lucas_cache_cap = new_cap;
        }
        /* compute next Lucas number */
        lucas_cache[lucas_cache_max + 1] =
            lucas_cache[lucas_cache_max] + lucas_cache[lucas_cache_max - 1];
        lucas_cache_max++;
    }
}

static seq_t lucas(idx_t m)
{
    ensure_lucas(m);
    return lucas_cache[m];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;

    /* n mod 3 */
    if ((n % 3) == 1) {
        return (seq_t)1;
    } else if ((n % 3) == 2) {
        /* n = 3k-1  →  k = (n+1)/3  →  index = 2k+1 = 2*(n+1)/3 + 1 */
        idx_t k   = (n + 1) / 3;
        idx_t idx = 2 * k + 1;
        return lucas(idx) - (seq_t)1;
    } else { /* n % 3 == 0, n = 3k */
        idx_t k   = n / 3;
        idx_t idx = 2 * k + 2;
        return lucas(idx) - (seq_t)1;
    }
}
