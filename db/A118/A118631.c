/*
 * A118631  Start with 1 and repeatedly reverse the digits and add 34 to get
 *          the next term.
 * https://oeis.org/A118631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n > 1, a(n) = rev(a(n‑1)) + 34
   (rev(x) = reverse the decimal digits of x) */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

/* Helper: reverse the decimal digits of a seq_t value */
static seq_t reverse_digits(seq_t x)
{
    seq_t rev = 0;
    while (x > (seq_t)0) {
        seq_t digit = x % (seq_t)10;
        rev = rev * (seq_t)10 + digit;
        x   = x / (seq_t)10;
    }
    return rev;
}

/* Sequence function */
seq_t a(idx_t n)
{
    assert(n >= (idx_t)1);          /* sequence is 1‑based with offset 1 */

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;
    static size_t   size  = 0;

    /* Initialize cache on first call */
    if (!cache) {
        cap  = 8;                    /* initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        cache[0] = (seq_t)1;         /* a(1) = 1 */
        size = 1;
    }

    /* Grow cache and compute missing terms up to n */
    while (size < (size_t)n) {
        seq_t prev = cache[size - 1];
        seq_t next = reverse_digits(prev) + (seq_t)34;

        if (size == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            cache = tmp;
            cap   = new_cap;
        }
        cache[size++] = next;
    }

    return cache[n - 1];   /* zero‑based index into cache */
}
