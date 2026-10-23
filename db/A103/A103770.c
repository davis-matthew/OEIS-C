/*
 * A103770  A weighted tribonacci sequence, (1,3,9).
 * https://oeis.org/A103770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + 9*a(n-3), a(0)=1, a(1)=1, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static idx_t a_cap   = 0;      /* allocated size of a_cache */
static idx_t a_computed = 0;   /* number of entries already computed */

/* ensure the cache can hold indices up to required (inclusive) */
static void ensure_capacity(idx_t required)
{
    if (required < a_cap) return;
    idx_t new_cap = required + 1;          /* need space for 0..required */
    seq_t *new_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL) {
        /* allocation failed – abort; for the purposes of this library an
           unchecked reallocation failure is acceptable as there is no
           recovery path. */
        abort();
    }
    a_cache = new_cache;
    a_cap   = new_cap;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* initialise base case cache if this is the first call */
    if (a_cache == NULL) {
        ensure_capacity(2);          /* indices 0,1,2 will be needed */
        a_cache[0] = 1;
        a_cache[1] = 1;
        a_cache[2] = 4;
        a_computed = 3;              /* 0,1,2 are computed */
    }

    if (n < 3) return a_cache[n];

    /* enlarge cache if we need more space */
    if (n >= a_cap) ensure_capacity(n);

    /* compute missing entries up to n */
    if (n >= a_computed) {
        for (idx_t i = a_computed; i <= n; ++i) {
            a_cache[i] = a_cache[i-1]
                        + (seq_t)3 * a_cache[i-2]
                        + (seq_t)9 * a_cache[i-3];
        }
        a_computed = n + 1;  /* now we have values up to index n */
    }

    return a_cache[n];
}
