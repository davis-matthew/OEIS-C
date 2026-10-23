/*
 * A130917  a(n) is the sum of the digital roots of all of the previous terms.
 * https://oeis.org/A130917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n)=sum_{k=1}^{n-1} digital_root(a(k)) */
/* Digital root is computed as 1 + ((x-1)%9) for x>0, 0 for x==0 */

#include <stdlib.h>   /* malloc, realloc, NULL, EXIT_FAILURE */

static seq_t *cache = NULL;      /* array of computed a(n), 1‑based */
static size_t  cap   = 0;        /* allocated capacity of cache */
static idx_t   last  = 0;        /* highest n that has been computed */
static seq_t   cum   = 0;        /* sum of digital roots of a(1)..a(last) */

static inline seq_t digital_root(seq_t x)
{
    if (x == 0) return 0;
    return (seq_t)(1 + ((x - 1) % 9));
}

/* Grow the cache to hold at least 'needed' (1‑based) entries. */
static void grow_cache(size_t needed)
{
    if (cap >= needed) return;
    size_t newcap = cap ? cap * 2 : 1;
    while (newcap < needed) newcap *= 2;
    seq_t *newp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!newp) {
        /* Allocation failed – terminate program (allowed for this helper). */
        exit(EXIT_FAILURE);
    }
    cache = newp;
    cap   = newcap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n<1, but harmless */

    if (last == 0) {      /* first call: initialise a(1)=1 */
        grow_cache(2);    /* ensure space for indices 1 and 2 */
        cache[1] = (seq_t)1;
        cum      = (seq_t)1;   /* digital_root(1) == 1 */
        last     = 1;
        if (n == 1) return cache[1];
    }

    if (n > last) {
        grow_cache(n + 1);  /* ensure space for index n */
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t val   = cum;                    /* a(i) = sum of previous drs */
            cache[i]    = val;
            seq_t dr    = digital_root(val);      /* digital root of a(i) */
            cum        += dr;                     /* update running sum */
            last        = i;
        }
    }

    return cache[n];
}
