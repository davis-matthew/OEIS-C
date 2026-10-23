/*
 * A041153  Denominators of continued fraction convergents to sqrt(86).
 * https://oeis.org/A041153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 20810*a(n-10) - a(n-20)
 * Initial values:
 *   a(0)=1, a(1)=3, a(2)=4, a(3)=7, a(4)=11,
 *   a(5)=95, a(6)=106, a(7)=201, a(8)=307, a(9)=1122,
 *   a(10)=20503, a(11)=62631, a(12)=83134, a(13)=145765,
 *   a(14)=228899, a(15)=1976957, a(16)=2205856,
 *   a(17)=4182813, a(18)=6388669, a(19)=23348820
 */

#include <stdlib.h>

/* Cached sequence values.  The cache is expanded as needed. */
static seq_t *seq_cache = NULL;
static idx_t seq_cache_size = 0;          /* capacity in elements */
static idx_t seq_cache_computed = (idx_t)-1; /* last index that has been computed */

/* First twenty known values of the sequence. */
static const seq_t seq_init[20] = {
    (seq_t)1,   (seq_t)3,   (seq_t)4,   (seq_t)7,   (seq_t)11,
    (seq_t)95,  (seq_t)106, (seq_t)201, (seq_t)307, (seq_t)1122,
    (seq_t)20503,(seq_t)62631,(seq_t)83134,(seq_t)145765,
    (seq_t)228899,(seq_t)1976957,(seq_t)2205856,(seq_t)4182813,
    (seq_t)6388669,(seq_t)23348820
};

/* Grow the cache to at least 'new_cap' elements.  Newly added slots
   are uninitialized; they will be computed on demand. */
static void ensure_cache_capacity(idx_t new_cap)
{
    if (new_cap <= seq_cache_size)
        return;

    idx_t cap = seq_cache_size ? seq_cache_size : 20;
    while (cap < new_cap)
        cap <<= 1;          /* double until enough */

    seq_t *new_ptr = (seq_t *)realloc(seq_cache, cap * sizeof(seq_t));
    if (!new_ptr) {
        /* In a real system we would handle the allocation failure. */
        abort();
    }
    seq_cache = new_ptr;
    seq_cache_size = cap;
}

/* Computes the terms up to 'n', storing them in the cache. */
static void compute_up_to(idx_t n)
{
    /* Initialise the cache if this is the first call. */
    if (!seq_cache) {
        ensure_cache_capacity(20);
        for (idx_t i = 0; i < 20; ++i)
            seq_cache[i] = seq_init[i];
        seq_cache_computed = 19;
    }

    if (n <= seq_cache_computed)
        return;

    /* Make sure the cache can hold 'n'. */
    ensure_cache_capacity(n + 1);

    /* Compute new terms using the recurrence. */
    for (idx_t i = seq_cache_computed + 1; i <= n; ++i) {
        /* For i >= 20, references are guaranteed to be already known. */
        seq_cache[i] = (seq_t)20810 * seq_cache[i - 10] - seq_cache[i - 20];
    }
    seq_cache_computed = n;
}

/* Main entry point for the sequence. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return seq_cache[n];
}
