/*
 * A041218  Numerators of continued fraction convergents to sqrt(120).
 * https://oeis.org/A041218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 22*a(n-2) – a(n-4);
   Initial values: a(0)=10, a(1)=11, a(2)=230, a(3)=241 */
#include <stdlib.h>

static seq_t *cache = NULL;      /* Memory for cached terms */
static size_t cap = 0;           /* Current allocation size (in seq_t elements) */
static idx_t last_computed = (idx_t)-1;   /* Largest index that has been computed */

/* Ensure that cache can hold indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        /* Allocate initial space for first four terms */
        cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (cache == NULL) exit(EXIT_FAILURE);  /* Allocation failed */
        /* Initialize the first four terms */
        cache[0] = (seq_t)10;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)230;
        cache[3] = (seq_t)241;
        last_computed = 3;
    }

    /* Grow the buffer if necessary */
    while (cap <= (size_t)n) {
        size_t new_cap = cap * 2;
        if (new_cap == 0) new_cap = 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) {
            free(cache);
            exit(EXIT_FAILURE);               /* Allocation failed */
        }
        cache = tmp;
        cap = new_cap;
    }
}

/* Gap‑free memoised computation of a(n) */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* If the requested value is already in the cache, return it */
    if (n <= last_computed) {
        return cache[n];
    }

    /* Compute missing terms one by one, storing them in the cache */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* For i >= 4, recurrence holds; for i <= 3 the terms are already stored */
        if (i >= 4) {
            cache[i] = 22 * cache[i - 2] - cache[i - 4];
        }
    }
    last_computed = n;

    return cache[n];
}
