/*
 * A097076  Expansion of g.f. x/(1 - x - 3*x^2 - x^3).
 * https://oeis.org/A097076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + a(n-3), a(0)=0, a(1)=1, a(2)=1
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ------------------------------------------------------------------------- */
/*  Helper: grow the cached array of values up to size 'needed' (indices 0..needed-1).   */
/* ------------------------------------------------------------------------- */
static void ensure_cache(idx_t needed)
{
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t size     = 0;   /* number of values cached (>= size) */

    /* Expand the array on demand. */
    if (needed < size) return;  /* already enough */

    if (capacity == 0) {
        capacity = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
    }
    while (capacity <= needed) {
        idx_t new_cap = capacity * 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return;  /* allocation failed; leave as is (caller will handle zero value) */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute values up to 'needed' by recurrence. */
    /* Base cases */
    if (size <= 0) { cache[0] = (seq_t)0; size = 1; }
    if (size <= 1) { cache[1] = (seq_t)1; size = 2; }
    if (size <= 2) { cache[2] = (seq_t)1; size = 3; }

    for (idx_t i = size; i <= needed; ++i) {
        cache[i] = cache[i-1] + (seq_t)3 * cache[i-2] + cache[i-3];
    }
    size = needed + 1;
    /* Store back for future calls */
    /* static variables maintain state */
    /* The following static variables are defined in this function as static. */
    /* They are accessible only here; but we need to expose cache to a() */
    /* So we declare them external by using a tiny struct.  */
    /* Instead, for simplicity, we store cache in a static global pointer. */
}

static seq_t *global_cache = NULL;
static idx_t global_capacity = 0;
static idx_t global_size = 0;

/* Reimplementation: we will store cache in global variables for a() to use. */
static void ensure_cache2(idx_t needed)
{
    if (needed < global_size) return;

    if (global_capacity == 0) {
        global_capacity = 16;
        global_cache = (seq_t *)malloc(sizeof(seq_t) * global_capacity);
    }
    while (global_capacity <= needed) {
        idx_t new_cap = global_capacity * 2;
        seq_t *tmp = (seq_t *)realloc(global_cache, sizeof(seq_t) * new_cap);
        if (!tmp) return;  /* allocation failed */
        global_cache = tmp;
        global_capacity = new_cap;
    }

    /* Initialize base values */
    if (global_size <= 0) { global_cache[0] = (seq_t)0; global_size = 1; }
    if (global_size <= 1) { global_cache[1] = (seq_t)1; global_size = 2; }
    if (global_size <= 2) { global_cache[2] = (seq_t)1; global_size = 3; }

    for (idx_t i = global_size; i <= needed; ++i) {
        global_cache[i] = global_cache[i-1] + (seq_t)3 * global_cache[i-2] + global_cache[i-3];
    }
    global_size = needed + 1;
}

/* ------------------------------------------------------------------------- */
/*  Public function implementing the sequence.                                 */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache2(n);
    return global_cache[n];
}
