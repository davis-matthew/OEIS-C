/*
 * A169985  Round phi^n to the nearest integer.
 * https://oeis.org/A169985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(phi^n) (offset 0) With a(0) = 1, a(1) = 2, and for
 *            n ≥ 2 a(n) = Lucas(n), where the Lucas sequence is 2, 1, 3, 4,
 *            7, 11, 18, …
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses memoised Lucas numbers. */

#include <stdlib.h>

static seq_t *lucas_cache   = NULL;   /* holds L[0], L[1], … */
static size_t lucas_size    = 0;      /* number of elements stored */
static size_t lucas_cap     = 0;      /* allocated capacity */

/* Ensure lucas_cache contains L[0] … L[n] */
static void ensure_lucas(idx_t n)
{
    if (lucas_size > (size_t)n)       /* already have required entries */
        return;

    /* Grow capacity to at least n+1, doubling each time */
    size_t new_cap = lucas_cap ? lucas_cap : 2;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;                /* multiply by 2 */

    seq_t *tmp = (seq_t *)realloc(lucas_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);          /* OOM */
    lucas_cache = tmp;
    lucas_cap   = new_cap;

    /* Initialise first two Lucas numbers if this is the first allocation */
    if (lucas_size == 0) {
        lucas_cache[0] = (seq_t)2;   /* L0 = 2   */
        lucas_cache[1] = (seq_t)1;   /* L1 = 1   */
        lucas_size    = 2;
    }

    /* Compute missing Lucas numbers up to index n */
    for (size_t i = lucas_size; i <= (size_t)n; ++i) {
        lucas_cache[i] = lucas_cache[i-1] + lucas_cache[i-2];
    }
    lucas_size = (size_t)n + 1;
}

/* Required function: returns A169985(n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* special first term  */
    if (n == 1) return (seq_t)2;   /* special second term */

    /* For n ≥ 2: use Lucas numbers */
    ensure_lucas(n);
    return lucas_cache[(size_t)n];
}
