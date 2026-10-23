/*
 * A135446  a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3), with a(0) = a(1) = -1 and
 *          a(2) = 3.
 * https://oeis.org/A135446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) a(0) = a(1) = -1; a(2) = 3
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 31, i64 n <= 62, i128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache   = NULL;
static size_t cap     = 0;   /* allocated capacity (number of slots)   */
static size_t computed = 0;  /* index of first slot not yet computed */

/* ensure that cache has at least `needed` slots (0‑based) */
static void ensure_capacity(size_t needed)
{
    if (needed < cap) return;
    size_t new_cap = needed + 1;
    if (new_cap < 16) new_cap = 16;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* out of memory */
    cache = tmp;
    cap   = new_cap;
}

/* compute all values from computed up to n (inclusive) */
static void compute_up_to(size_t n)
{
    if (n < computed) return;
    for (size_t i = computed; i <= n; ++i) {
        if (i == 0 || i == 1)
            cache[i] = (seq_t)-1;
        else if (i == 2)
            cache[i] = (seq_t)3;
        else
            cache[i] = 3 * cache[i-1] - 3 * cache[i-2] + 2 * cache[i-3];
    }
    computed = n + 1;
}

seq_t a(idx_t n)
{
    size_t sn = (size_t)n;
    ensure_capacity(sn);
    compute_up_to(sn);
    return cache[sn];
}
