/*
 * A041778  Numerators of continued fraction convergents to sqrt(410).
 * https://oeis.org/A041778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041778: Numerators of continued fraction convergents to sqrt(410)
   Recurrence: a(n) = 162*a(n-2) - a(n-4) (n >= 4)
   Initial values: a(0)=20, a(1)=81, a(2)=3260, a(3)=13121 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values of the sequence   */
static idx_t  len   = 0;             /* number of elements currently cached */
static size_t cap   = 0;             /* allocated capacity of cache       */

/* Initialise the base values in the cache. */
static void init_base(void)
{
    if (cache != NULL) return;      /* already initialised */

    cap = 4;
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cache) exit(1);            /* allocation failure – abort */

    cache[0] = (seq_t)20;
    cache[1] = (seq_t)81;
    cache[2] = (seq_t)3260;
    cache[3] = (seq_t)13121;
    len  = 4;
}

/* Ensure that the cache has room for at least (n+1) elements. */
static void ensure_capacity(idx_t n)
{
    if (cap >= (size_t)n + 1) return;

    size_t new_cap = cap * 2;
    if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);              /* allocation failure – abort */

    cache = tmp;
    cap   = new_cap;
}

/* Compute the n-th term of the sequence. */
seq_t a(idx_t n)
{
    init_base();

    if (n < len)                /* already computed */
        return cache[n];

    ensure_capacity(n);

    /* Fill the cache from 'len' up to and including 'n'. */
    for (idx_t i = len; i <= n; ++i) {
        cache[i] = (seq_t)162 * cache[i - 2] - cache[i - 4];
    }

    len = n + 1;                 /* all terms up to 'n' are now available */
    return cache[n];
}
