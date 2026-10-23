/*
 * A023441  Dying rabbits: a(n) = a(n-1) + a(n-2) - a(n-11).
 * https://oeis.org/A023441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=5,
   a(6)=8, a(7)=13, a(8)=21, a(9)=34, a(10)=55
   a(n) = a(n-1) + a(n-2) - a(n-11)  (for n >= 11, offset 0) */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* cache of calculated terms */
static size_t cap   = 0;        /* number of allocated entries */

/* Ensure cache holds values up to index n and all entries are initialized. */
static void ensure_cache(idx_t n)
{
    /* If already large enough, nothing to do */
    if ((size_t)n + 1 <= cap) return;

    /* Allocate (or grow) the cache */
    size_t new_cap = (size_t)n + 1;
    if (new_cap < 16) new_cap = 16;                /* small initial size */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                            /* allocation failure */
    cache = tmp;

    /* Compute any new terms in order, using the recurrence. */
    for (size_t i = cap; i < new_cap; ++i) {
        seq_t val;
        switch (i) {
            case 0:  val = 0;  break;
            case 1:  val = 1;  break;
            case 2:  val = 1;  break;
            case 3:  val = 2;  break;
            case 4:  val = 3;  break;
            case 5:  val = 5;  break;
            case 6:  val = 8;  break;
            case 7:  val = 13; break;
            case 8:  val = 21; break;
            case 9:  val = 34; break;
            case 10: val = 55; break;
            default:
                /* recurrence: a(n) = a(n‑1)+a(n‑2)−a(n‑11) */
                val = cache[i-1] + cache[i-2] - cache[i-11];
                break;
        }
        cache[i] = val;
    }
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* fast path for the first term */
    ensure_cache(n);
    return cache[(size_t)n];
}
